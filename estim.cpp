#include "estim.h"
#include "ui_estim.h"

Estim::Estim(QWidget *parent) : QWidget(parent), ui(new Ui::Estim){
    ui->setupUi(this);

    connect(ui->cBEstActiv, &QCheckBox::clicked, this, &Estim::slotIsActive);
    connect(ui->cBEstType, SIGNAL(currentIndexChanged(int)), this, SLOT(slotParamChanged(int)));
    // --- lineEdits ---
    connect(ui->lEMult, SIGNAL(textChanged(QString)), this, SLOT(slotInputChanged(QString)));
    connect(ui->lERef, SIGNAL(textChanged(QString)), this, SLOT(slotInputChanged(QString)));
    connect(ui->cBSuff, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSuffChanged(int)));

    valMult = new QDoubleValidator(1, 5, 2, this);
    valMult->setRange(0, 99999.999, 2);
    ui->lEMult->setValidator(valMult);

    valRef = new QDoubleValidator(1, 2, 2, this);
    valRef->setRange(0, 10.0, 2);
    ui->lERef->setValidator(valRef);

    // --- final actions ---
    ui->cBSuff->setCurrentIndex(4);
    emit ui->cBEstActiv->clicked(false);
}

Estim::~Estim(){
    delete valMult;
    delete valRef;

    delete ui;
}

void Estim::setParamNum(uint8_t num){
    ui->gBEstim->setTitle(ui->gBEstim->title() + " " + QString::number(num + 1));
}

QJsonObject Estim::toJsonObject(){
    QJsonObject jEstim;
    jEstim[keyType] = ui->cBEstType->currentIndex();
    jEstim[keyMult] = ui->lEMult->text();
    jEstim[keyRef] = ui->lERef->text();
    jEstim[keySuff] = ui->cBSuff->currentIndex();
    jEstim[keyIsAct] = ui->cBEstActiv->isChecked();
    return jEstim;
}

void Estim::uiFromJson(const QJsonObject &jUi){
    ui->cBEstType->setCurrentIndex(jUi[keyType].toInt());
    ui->lEMult->setText(jUi[keyMult].toString());
    ui->lERef->setText(jUi[keyRef].toString());
    ui->cBSuff->setCurrentIndex(jUi[keySuff].toInt());
    ui->cBEstActiv->setChecked(jUi[keyIsAct].toBool());
    emit ui->cBEstActiv->clicked(jUi[keyIsAct].toBool());
}

void Estim::uiLockUnLock(bool isLogging){
    if(isLogging){ // lock
        slotIsActive(false);
        ui->cBEstActiv->setEnabled(false);
        ui->gBEstim->setEnabled(false);
    }
    else{ // unlock
        ui->gBEstim->setEnabled(true);
        ui->cBEstActiv->setEnabled(true);
        slotIsActive(ui->cBEstActiv->isChecked());        
    }
}

void Estim::slotIsActive(bool isAct){
    ui->cBEstType->setEnabled(isAct);    
    ui->lEMult->setEnabled(isAct);
    for(int i = 0; i < ui->gLEstText->columnCount(); i++){
        for(int j = 0; j < ui->gLEstText->rowCount(); j++){
            QLayoutItem *loutWidget = ui->gLEstText->itemAtPosition(j, i);
            if(loutWidget){
                QWidget *w = static_cast<QWidget*>(loutWidget->widget());
                w->setEnabled(isAct);
            }
        }
    }
}

void Estim::slotParamChanged(int ind){
    if(ind == 0){
        ui->lEMult->show();
    }
    else if(ind == 1){
        ui->lEMult->hide();
    }
    else if(ind == 2){
        ui->lEMult->hide();
    }
    else if(ind == 3){
        ui->lEMult->hide();
    }
    else if(ind == 4){
        ui->lEMult->hide();
    }
}

void Estim::slotSuffChanged(int ind){
    // FIXME: ? void Estim::slotSuffChanged(int ind)
}

void Estim::slotInputChanged(const QString &newTxt){
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
