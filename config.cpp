#include "config.h"
#include "ui_config.h"

Config::Config(QWidget *parent) : QWidget(parent), ui(new Ui::Config){
    ui->setupUi(this);

    ui->dialTimDiv->setMaximum(ui->cBTimDiv->count() - 1);
    ui->dialTimDiv->setStyleSheet("background-color: rgb(240, 240, 0)");

    for (uint8_t i = 0; i < MAX_CH_NUM; i++)
        ui->cBTrigSrc->addItem("CH" + QString::number(i + IND_TO_NUM));
}

Config::~Config(){
    delete ui;
}
