#include "config.h"
#include "ui_config.h"

Config::Config(QWidget *parent) : QWidget(parent), ui(new Ui::Config){
    ui->setupUi(this);

    ui->dialTimDiv->setMaximum(ui->cBTimDiv->count() - 1);
    ui->dialTimDiv->setStyleSheet("background-color: rgb(240, 240, 0)");

    for (uint8_t i = 0; i < MAX_CH_NUM; i++)
        ui->cBTrigSrc->addItem("CH" + QString::number(i + IND_TO_NUM));

    // --- connections ---
    connect(ui->cBTimDiv, &QComboBox::currentIndexChanged, this, &Config::slotTimDiv);
    connect(ui->cBTrigSrc, &QComboBox::currentIndexChanged, this, &Config::slotTrigSrc);
    connect(ui->cBPulsePolar, &QComboBox::currentIndexChanged, this, &Config::slotTrigSlope);
}

Config::~Config(){
    delete ui;
}

void Config::setCount(uint8_t num){
    ui->gBHantek->setTitle(ui->gBHantek->title() + " " + QString::number(num));
}

void Config::updUIAfterModel(){
    ui->cBTrigSrc->setCurrentIndex(controlData->nTriggerSource);
    ui->cBTimDiv->setCurrentIndex(controlData->nTimeDIV);
    ui->cBPulsePolar->setCurrentIndex(controlData->nTriggerSlope);
    // TODO: (HOME) void Config::updUIAfterModel()
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
