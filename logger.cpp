#include "logger.h"
#include "ui_logger.h"

Logger::Logger(QWidget *parent) : QDialog(parent), ui(new Ui::Logger){
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Logger::slotTimerTick);
    timer->start(200);

    connect(ui->pBLogAddSymb, &QPushButton::clicked, this, &Logger::slotAddSymb);
    connect(ui->pBLogDir, SIGNAL(clicked()), this, SLOT(slotChooseDir()));
}

Logger::~Logger(){
    delete ui;
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
}

void Logger::slotAddSymb(){
    // TODO: ...
}

void Logger::slotChooseDir(){
    /*QString tempStr = QFileDialog::getExistingDirectory(this,
                                                        tr("Save log to..."), commonDirPath,
                                                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(!tempStr.isEmpty()){
        if(tempStr.back() == '/')
            tempStr.chop(1);
        commonDirPath = tempStr;
        tLogger.strDirPath = tempStr;
        ui->lPathSaveData->setText("Path: " + tLogger.getFullName());
    }*/
}
