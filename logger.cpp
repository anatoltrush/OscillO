#include "logger.h"
#include "ui_logger.h"

Logger::Logger(QWidget *parent) : QDialog(parent), ui(new Ui::Logger){
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Logger::slotTimerTick);
    timer->start(200);

    connect(ui->pBLogAddSymb, &QPushButton::clicked, this, &Logger::slotAddSymb);
    connect(ui->pBLogDir, SIGNAL(clicked()), this, SLOT(slotChooseDir()));
    connect(ui->cBLogOnOff, SIGNAL(toggled(bool)), this, SLOT(slotOnOff(bool)));

    connect(ui->lELogTempRoom, SIGNAL(textChanged(QString)), this, SLOT(slotInputChanged(QString)));
    connect(ui->lELogHumid, SIGNAL(textChanged(QString)), this, SLOT(slotInputChanged(QString)));
    connect(ui->lELogTempEnv, SIGNAL(textChanged(QString)), this, SLOT(slotInputChanged(QString)));
    connect(ui->lELogCondEnv, SIGNAL(textChanged(QString)), this, SLOT(slotInputChanged(QString)));

    // --- logger ---
    tLogger.strDirPath = QDir::currentPath();
    tLogger.updFileName("/HANTEK_DATA_");
    ui->lLogPath->setText("Path: " + tLogger.getFullName());
    // --- threads ---
    thrLog = std::thread(&TechLogger::dropData, &tLogger, &condVar);

    // --- validators ---
    valTroom = new QDoubleValidator(1, 2, 1, this);
    valTroom->setRange(0, 60, 1);
    ui->lELogTempRoom->setValidator(valTroom);

    valHum = new QDoubleValidator(1, 3, 1, this);
    valHum->setRange(0, 100, 1);
    ui->lELogHumid->setValidator(valHum);

    valTenv = new QDoubleValidator(1, 3, 1, this);
    valTenv->setRange(0, 100, 1);
    ui->lELogTempEnv->setValidator(valTenv);

    valCond = new QDoubleValidator(1, 5, 2, this);
    valCond->setRange(0, 50000, 2);
    ui->lELogCondEnv->setValidator(valCond);
}

Logger::~Logger(){
    tLogger.isRunning = false;
    if(thrLog.joinable()) thrLog.join();

    delete valTroom;
    delete valHum;
    delete valTenv;
    delete valCond;

    delete ui;
}

void Logger::receiveAndSaveLine(const QString &logLine){
    std::unique_lock<std::mutex> lockRadRaw(mutLoc);
    tLogger.data.push_back(logLine.toStdString());
}

void Logger::slotTimerTick(){
    if(ui->cBLogOnOff->isChecked()){
        uint8_t len = 10;
        QString strIndic = QString(len, '-');
        ui->lLogIndicator->setStyleSheet("background-color: green");
        strIndic.replace(indicCount, 1, ">");
        indicCount++;
        if(indicCount >= len - 1) indicCount = 1;
        strIndic += "[.log]";
        ui->lLogIndicator->setText(strIndic);
    }
    else{
        ui->lLogIndicator->setStyleSheet("background-color: red");
        ui->lLogIndicator->setText("-X-");
    }

    // ---
    if(ui->lELogTempRoom->text().isEmpty()) ui->lELogTempRoom->setText("0");
    if(ui->lELogHumid->text().isEmpty()) ui->lELogHumid->setText("0");
    if(ui->lELogTempEnv->text().isEmpty()) ui->lELogTempEnv->setText("0");
    if(ui->lELogCondEnv->text().isEmpty()) ui->lELogCondEnv->setText("0");
}

void Logger::slotAddSymb(){
    tLogger.addSymb(ui->lELogSymb->text());
    ui->lLogPath->setText("Path: " + tLogger.getFullName());
}

void Logger::slotChooseDir(){
    QString tempStr = QFileDialog::getExistingDirectory(this,
                                                        tr("Save log to..."), commonDirPath,
                                                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(!tempStr.isEmpty()){
        if(tempStr.back() == '/') tempStr.chop(1);
        commonDirPath = tempStr;
        tLogger.strDirPath = tempStr;
        ui->lLogPath->setText("Path: " + tLogger.getFullName());
    }
}

void Logger::slotOnOff(bool isChecked){
    isLogging = isChecked;
    if(isChecked){
        // --- lock UI ---
        ui->lELogTempRoom->setEnabled(false);
        ui->lELogHumid->setEnabled(false);
        ui->lELogTempEnv->setEnabled(false);
        ui->lELogCondEnv->setEnabled(false);

        ui->lELogSymb->setEnabled(false);
        ui->pBLogAddSymb->setEnabled(false);
        ui->pBLogDir->setEnabled(false);
        ui->pTELogComment->setEnabled(false);
    }
    else{
        tLogger.updFileName("/HANTEK_DATA_");
        ui->lLogPath->setText("Path: " + tLogger.getFullName());
        // --- unlock UI ---
        ui->lELogTempRoom->setEnabled(true);
        ui->lELogHumid->setEnabled(true);
        ui->lELogTempEnv->setEnabled(true);
        ui->lELogCondEnv->setEnabled(true);

        ui->lELogSymb->setEnabled(true);
        ui->pBLogAddSymb->setEnabled(true);
        ui->pBLogDir->setEnabled(true);
        ui->pTELogComment->setEnabled(true);
    }

    emit signLoggerWork(isChecked);
}

void Logger::slotInputChanged(const QString& newTxt){
    QLineEdit *LEdit = qobject_cast<QLineEdit*>(sender());
    QString tmpLEdit = LEdit->text();

    int pos = 0;
    const QValidator* locValid = LEdit->validator();
    if(locValid){
        QValidator::State st = locValid->validate(tmpLEdit, pos);
        tmpLEdit.chop(1);
        st == QValidator::Acceptable ? LEdit->setText(newTxt) : LEdit->setText(tmpLEdit);
    }
}
