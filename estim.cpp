#include "estim.h"
#include "ui_estim.h"

Estim::Estim(QWidget *parent) : QWidget(parent), ui(new Ui::Estim){
    ui->setupUi(this);

    connect(ui->cBEstActiv, &QCheckBox::clicked, this, &Estim::slotIsActive);
    connect(ui->cBEstType, SIGNAL(currentIndexChanged(int)), this, SLOT(slotParamChanged(int)));
    // --- ledits ---
    connect(ui->lEMult, SIGNAL(textChanged(QString)), this, SLOT(slotCondChanged(QString)));
    connect(ui->lERef, SIGNAL(textChanged(QString)), this, SLOT(slotCondChanged(QString)));

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

    jEstim[keyIsAct] = ui->cBEstActiv->isChecked();
    jEstim[keyType] = ui->cBEstType->currentIndex();
    jEstim[keyMult] = ui->lEMult->text();
    jEstim[keyRef] = ui->lERef->text();
    jEstim[keySuff] = ui->cBSuff->currentIndex();

    return jEstim;
}

void Estim::slotIsActive(bool isAct){
    ui->cBEstType->setEnabled(isAct);    
    ui->lEMult->setEnabled(isAct);
    for(int i = 0; i < ui->gLEstText->columnCount(); i++){
        for(int j = 0; j < ui->gLEstText->rowCount(); j++){
            QWidget *w = static_cast<QWidget*>(ui->gLEstText->itemAtPosition(j, i)->widget());
            w->setEnabled(isAct);
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

void Estim::slotCondChanged(const QString &newTxt){
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
