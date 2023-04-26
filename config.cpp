#include "config.h"
#include "ui_config.h"

Config::Config(QWidget *parent) : QWidget(parent), ui(new Ui::Config){
    ui->setupUi(this);

    ui->dialTimDiv->setMaximum(ui->cBTimDiv->count() - 1);
    ui->dialTimDiv->setStyleSheet("background-color: rgb(0, 0, 40)");

    for (uint8_t i = 0; i < MAX_CH_NUM; i++)
        ui->cBTrigSrc->addItem("CH" + QString::number(i + IND_TO_NUM));

    QLocale locale(QLocale::English);
    valPulseWid = new QIntValidator(0, 999, this);
    valPulseWid->setRange(0, 999999);
    valPulseWid->setLocale(locale);
    ui->lEPulseWid->setValidator(valPulseWid);

    // --- connections ---
    connect(ui->cBFormat, SIGNAL(currentIndexChanged(int)), this, SLOT(slotFormat(int)));
    connect(ui->cBTimDiv, SIGNAL(currentIndexChanged(int)), this, SLOT(slotTimDiv(int)));
    connect(ui->cBTrigMode, SIGNAL(currentIndexChanged(int)), this, SLOT(slotTrigMode(int)));
    connect(ui->cBTrigSweep, SIGNAL(currentIndexChanged(int)), this, SLOT(slotTrigSweep(int)));
    connect(ui->cBTrigSrc, SIGNAL(currentIndexChanged(int)), this, SLOT(slotTrigSrc(int)));
    connect(ui->cBPulsePolar, SIGNAL(currentIndexChanged(int)), this, SLOT(slotTrigSlope(int)));
    connect(ui->cBBufLen, SIGNAL(currentTextChanged(QString)), this, SLOT(slotBufLen(QString)));

    connect(ui->cBPwCond, SIGNAL(currentIndexChanged(int)), this, SLOT(slotPWCond(int)));
    connect(ui->cBPulseUnit, SIGNAL(currentIndexChanged(int)), this, SLOT(slotPulseUnit(int)));
    connect(ui->lEPulseWid, SIGNAL(textChanged(QString)), this, SLOT(slotInpPulseWid()));
}

Config::~Config(){
    delete valPulseWid;
    delete ui;
}

void Config::setCount(uint8_t num){
    ui->gBHantek->setTitle(ui->gBHantek->title() + " " + QString::number(num));
}

void Config::updUIAfterModel(){
    ui->cBFormat->setCurrentIndex(extraConfig->m_nYTFormat);
    ui->cBTimDiv->setCurrentIndex(controlData->nTimeDIV);
    ui->cBTrigMode->setCurrentIndex(extraConfig->m_nTriggerMode);
    ui->cBTrigSweep->setCurrentIndex(extraConfig->m_nTriggerSweep);
    ui->cBTrigSrc->setCurrentIndex(controlData->nTriggerSource);        
    ui->cBPulsePolar->setCurrentIndex(controlData->nTriggerSlope);
    ui->cBPwCond->setCurrentIndex(extraConfig->pwCondInd);
    ui->cBPulseUnit->setCurrentIndex(extraConfig->pulseWidUnit);
    ui->lEPulseWid->setText(QString::number(extraConfig->pulseWidVal));
    emit ui->cBTrigMode->currentIndexChanged(extraConfig->m_nTriggerMode);
    // ---
    int rightInd = 0;
    for (int i = 0; i < ui->cBBufLen->count(); i++){
        QString currTxt = ui->cBBufLen->itemText(i);
        QList<QString> lst = currTxt.split("->");
        if(lst.size() != 2) continue;
        QString strLong = lst.front().replace(" ", "");
        ulong resUlong = strLong.toULong();
        if (resUlong == controlData->nBufferLen)
            rightInd = i;
    }
    ui->cBBufLen->setCurrentIndex(0);
    ui->cBBufLen->setCurrentIndex(1);
    ui->cBBufLen->setCurrentIndex(rightInd);
}

void Config::uiLockUnLock(bool isLogging){
    ui->gBTrigger->setEnabled(!isLogging);
    ui->gBHorizontal->setEnabled(!isLogging);
    // ---
    ui->dialTimDiv->setEnabled(!isLogging);
    // ---
    for(int i = 0; i < ui->vLTDiv->count(); i++){
        QLayoutItem *loutWidget = ui->vLTDiv->itemAt(i);
        if(loutWidget){
            QWidget *w = static_cast<QWidget*>(loutWidget->widget());
            if(w) w->setEnabled(!isLogging);
        }
    }
    // ---
    for(int i = 0; i < ui->gLTrigger->columnCount(); i++){
        for(int j = 0; j < ui->gLTrigger->rowCount(); j++){
            QLayoutItem *loutWidget = ui->gLTrigger->itemAtPosition(j, i);
            if(loutWidget){
                QWidget *w = static_cast<QWidget*>(loutWidget->widget());
                if(w) w->setEnabled(!isLogging);
            }
        }
    }
}

void Config::slotTrigMode(int ind){
    extraConfig->m_nTriggerMode = ind;
    if(ind == 1){
        ui->lPWCond->show();
        ui->lPulseWid->show();
        ui->cBPwCond->show();
        ui->lEPulseWid->show();
        ui->cBPulseUnit->show();
    }
    else{
        ui->lPWCond->hide();
        ui->lPulseWid->hide();
        ui->cBPwCond->hide();
        ui->lEPulseWid->hide();
        ui->cBPulseUnit->hide();
    }
}

void Config::slotBufLen(const QString &strLen){
    QList<QString> lst = strLen.split("->");
    if(lst.size() != 2){
        QMessageBox::critical(this, "Error", "Bad buffer length parse");
        return;
    }
    // ---
    QString strLong = lst.front().replace(" ", "");
    ulong resUlong = strLong.toULong();
    if(resUlong < 2050) // FIXME: delete later
        controlData->nBufferLen = controlData->nReadDataLen = resUlong;
}

void Config::slotInpPulseWid(){
    QLineEdit *LEdit = qobject_cast<QLineEdit*>(sender());
    QString tmpLEdit = LEdit->text();
    if(tmpLEdit.contains(".")) tmpLEdit.replace(".", "");
    if(tmpLEdit.contains(",")) tmpLEdit.replace(",", "");

    extraConfig->pulseWidVal = tmpLEdit.toInt();
    LEdit->setText(tmpLEdit);
}
