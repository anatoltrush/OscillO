#include "estim.h"
#include "ui_estim.h"

Estim::Estim(QWidget *parent) : QWidget(parent), ui(new Ui::Estim){
    ui->setupUi(this);

    connect(ui->cBEstActiv, &QCheckBox::clicked, this, &Estim::slotIsActive);
    connect(ui->cBEstType, SIGNAL(currentIndexChanged(int)), this, SLOT(slotParamChanged(int)));

    // --- final actions ---
    emit ui->cBEstActiv->clicked(false);
}

Estim::~Estim(){
    delete ui;
}

void Estim::setParamNum(uint8_t num){
    ui->gBEstim->setTitle(ui->gBEstim->title() + " " + QString::number(num + 1));
}

void Estim::slotIsActive(bool isAct){
    ui->cBEstType->setEnabled(isAct);
    ui->gLEstText->setEnabled(isAct);
}

void Estim::slotParamChanged(int ind){
    if(ind == 0){

    }
    else if(ind == 1){

    }
    // ...
}
