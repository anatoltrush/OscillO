#include "config.h"
#include "ui_config.h"

Config::Config(QWidget *parent) : QWidget(parent), ui(new Ui::Config){
    ui->setupUi(this);

    ui->dialTimDiv->setMaximum(ui->cBTimDiv->count() - 1);
    ui->dialTimDiv->setStyleSheet("background-color: rgb(240, 240, 0)");

    for (uint8_t i = 0; i < MAX_CH_NUM; i++)
        ui->cBTrigSrc->addItem("CH" + QString::number(i + IND_TO_NUM));

    // --- connections ---
    connect(ui->cBFormat, SIGNAL(currentIndexChanged(int)), this, SLOT(slotFormat(int)));
    connect(ui->cBTimDiv, SIGNAL(currentIndexChanged(int)), this, SLOT(slotTimDiv(int)));
    connect(ui->cBTrigMode, SIGNAL(currentIndexChanged(int)), this, SLOT(slotTrigMode(int)));
    connect(ui->cBTrigSweep, SIGNAL(currentIndexChanged(int)), this, SLOT(slotTrigSweep(int)));
    connect(ui->cBTrigSrc, SIGNAL(currentIndexChanged(int)), this, SLOT(slotTrigSrc(int)));
    connect(ui->cBPulsePolar, SIGNAL(currentIndexChanged(int)), this, SLOT(slotTrigSlope(int)));
}

Config::~Config(){
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
            w->setEnabled(!isLogging);
        }
    }
    // ---
    for(int i = 0; i < ui->gLTrigger->columnCount(); i++){
        for(int j = 0; j < ui->gLTrigger->rowCount(); j++){
            QLayoutItem *loutWidget = ui->gLTrigger->itemAtPosition(j, i);
            if(loutWidget){
                QWidget *w = static_cast<QWidget*>(loutWidget->widget());
                w->setEnabled(!isLogging);
            }
        }
    }
}
