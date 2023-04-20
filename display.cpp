#include "display.h"
#include "ui_display.h"

int Display::pseudoHTPos = 0.0;

Display::Display(QWidget *parent) : QWidget(parent), ui(new Ui::Display){
    ui->setupUi(this);

    // --- start actions ---
    ui->dialVoltDiv->setMaximum(ui->cBVoltDiv->count() - 1);
    ui->dialVoltDiv->setStyleSheet("background-color: rgb(240, 240, 0)");

    ui->tWChannel->setCurrentIndex(0);

    for (uint8_t i = 0; i < MAX_CH_NUM; i++)
        ui->cBChannel->addItem("CH" + QString::number(i + IND_TO_NUM));

    // --- estim panels ---
    for(uint8_t i = 0; i < ESTIM_NUM; i++){
        estims[i] = new Estim(this);
        estims[i]->setParamNum(i);
        ui->vLEstim->addWidget(estims[i]);
    }

    // --- chart ---
    chart = new QChart();
    chart->legend()->hide();
    chart->layout()->setContentsMargins(0, 0, 0, 0);
    chart->setMargins(QMargins(0, 0, 0, 0));

    chartView = new QChartView(chart);
    ui->gLChart->addWidget(chartView, 2, 1);

    // --- connections ---
    connect(ui->cBOnOff, SIGNAL(clicked(bool)), this, SLOT(slotChannOnOff(bool)));
    connect(ui->cBChannel, SIGNAL(currentIndexChanged(int)), this, SLOT(slotUpdateUiChannel()));
    connect(ui->cBVoltDiv, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChannTimDiv(int)));
    connect(ui->cBCoupling, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChannCoupl(int)));
    connect(ui->cBMult, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChannMult(int)));
    // --- sliders ---
    connect(ui->vSVertPos, &QSlider::valueChanged, this, &Display::slotLevelPos);
    connect(ui->vSTrigVert, &QSlider::valueChanged, this, &Display::slotTrigVert);
    connect(ui->hSTrigHor, &QSlider::valueChanged, this, &Display::slotTrigHor);

    // --- final actions ---
    Frame locFrm;
    showInChart(locFrm);
}

Display::~Display(){
    delete ui;
}

void Display::chooseChannel(uint8_t ch){
    if(ch >= MAX_CH_NUM) return;
    ui->cBChannel->setCurrentIndex(ch);
}

void Display::slotUpdateUiChannel(){
    currChannInd = ui->cBChannel->currentIndex();
    // --- relay control ---
    ui->cBVoltDiv->setCurrentIndex(relayControl->nCHVoltDIV[currChannInd]);
    ui->cBCoupling->setCurrentIndex(relayControl->nCHCoupling[currChannInd]);
    ui->cBMult->setCurrentIndex(relayControl->nCHMult[currChannInd]);
    // ---
    if(relayControl->bCHEnable[currChannInd]){ // ON
        ui->cBOnOff->setChecked(true);
        ui->cBVoltDiv->setEnabled(true);
        ui->cBCoupling->setEnabled(true);
        ui->cBMult->setEnabled(true);
        ui->dialVoltDiv->setEnabled(true);
    }
    else{ // OFF
        ui->cBOnOff->setChecked(false);
        ui->cBVoltDiv->setEnabled(false);
        ui->cBCoupling->setEnabled(false);
        ui->cBMult->setEnabled(false);
        ui->dialVoltDiv->setEnabled(false);
        chart->removeAllSeries();
    }
    // --- control data ---
    ui->vSTrigVert->setValue(controlData->nVTriggerPos);
    ui->hSTrigHor->setValue(pseudoHTPos);
    // --- extra config ---
    ui->vSVertPos->setValue(extraConfig->m_nLeverPos[currChannInd]);
}

void Display::updateUiLinear(float perc){
    linerPos = extraConfig->lastPLSize * perc;
}

void Display::slotChannOnOff(bool isOn){
    relayControl->bCHEnable[ui->cBChannel->currentIndex()] = isOn;
    emit signChannelStateChanged();
}

void Display::slotChannTimDiv(int ind){
    relayControl->nCHVoltDIV[ui->cBChannel->currentIndex()] = ind;
    emit signChannelStateChanged();
}

void Display::slotChannCoupl(int ind){
    relayControl->nCHCoupling[ui->cBChannel->currentIndex()] = ind;
    emit signChannelStateChanged();
}

void Display::slotChannMult(int ind){
    relayControl->nCHMult[ui->cBChannel->currentIndex()] = ind;
    emit signChannelStateChanged();
}

void Display::slotLevelPos(int val){
    extraConfig->m_nLeverPos[ui->cBChannel->currentIndex()] = val;
    emit signChannelStateChanged();
}

void Display::slotTrigVert(int val){
    if(!controlData) return;
    controlData->nVTriggerPos = val;
    emit signChannelStateChanged();
}

void Display::slotTrigHor(int val){
    if(!controlData) return;
    pseudoHTPos = val;
    controlData->nHTriggerPos = (uint8_t)((val / (float)extraConfig->lastPLSize) * 100);
    emit signChannelStateChanged();
}

void Display::showInChart(const Frame &frame){
    payLoadSize = frame.payload.size();
    if(extraConfig && payLoadSize != 0){
        ui->hSTrigHor->setMaximum(payLoadSize);
        extraConfig->lastPLSize = payLoadSize;
    }

    // --- axis X ---
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, frame.payload.size() - IND_TO_NUM);
    axisX->setLabelFormat("%d");
    axisX->setMinorTickCount(1);
    chart->setAxisX(axisX);
    // --- axis Y ---
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, MAX_VERT_AXIS);
    axisY->setLabelFormat("%d");
    axisY->setMinorTickCount(1);
    chart->setAxisY(axisY);

    chart->removeAllSeries();
    // --- main data ---
    QLineSeries* serData = new QLineSeries();
    for (size_t j = 0; j < frame.payload.size(); j++)
        serData->append(j, frame.payload[j]);
    chart->addSeries(serData);
    // --- ruler ---
    QLineSeries* serRuler = new QLineSeries();
    serRuler->append(linerPos, 0);
    serRuler->append(linerPos, MAX_VERT_AXIS);
    chart->addSeries(serRuler);
    // --- lev pos ---
    QLineSeries* serLevPos = new QLineSeries();
    if(extraConfig){
        serLevPos->append(0, extraConfig->m_nLeverPos[currChannInd]);
        serLevPos->append(payLoadSize, extraConfig->m_nLeverPos[currChannInd]);
    }
    chart->addSeries(serLevPos);
    // --- vert trig ---
    QLineSeries* serVertTrig = new QLineSeries();
    if(controlData){
        serVertTrig->append(0, controlData->nVTriggerPos);
        serVertTrig->append(payLoadSize, controlData->nVTriggerPos);
    }
    chart->addSeries(serVertTrig);
    // --- hor trig ---
    QLineSeries* serHorTrig = new QLineSeries();
    if(controlData){
        serHorTrig->append(pseudoHTPos, 0);
        serHorTrig->append(pseudoHTPos, MAX_VERT_AXIS);
    }
    chart->addSeries(serHorTrig);

    // --- axes ---
    serData->attachAxis(axisX);
    serData->attachAxis(axisY);
    // ---
    serRuler->attachAxis(axisX);
    serRuler->attachAxis(axisY);
    // ---
    serLevPos->attachAxis(axisX);
    serLevPos->attachAxis(axisY);
    // ---
    serVertTrig->attachAxis(axisX);
    serVertTrig->attachAxis(axisY);
    // ---
    serHorTrig->attachAxis(axisX);
    serHorTrig->attachAxis(axisY);
}

QJsonObject Display::toJsonObject(){
    QJsonObject jDisplay;

    jDisplay[keyCurrCh] = ui->cBChannel->currentIndex();

    QJsonArray jEstims;
    for(uint8_t i = 0; i < ESTIM_NUM; i++)
        jEstims.push_back(estims[i]->toJsonObject());
    jDisplay[keyEstims] = jEstims;

    return jDisplay;
}

void Display::uiFromJson(const QJsonObject &jUi){
    if(jUi.isEmpty()){
        QMessageBox::critical(this, "Error", "Bad json input (Display data)");
        return;
    }
    // ---
    QJsonArray jArrEstims = jUi[keyEstims].toArray();
    if(jArrEstims.size() != ESTIM_NUM){
        QMessageBox::critical(this, "Error", "Bad json input data. NofEstims != 3");
        return;
    }
    // ---===---
    ui->cBChannel->setCurrentIndex(0);
    ui->cBChannel->setCurrentIndex(1);
    ui->cBChannel->setCurrentIndex(jUi[keyCurrCh].toInt());
    // ---
    for(uint8_t i = 0; i < ESTIM_NUM; i++)
        estims[i]->uiFromJson(jArrEstims[i].toObject());
    // --- pay load ---
    payLoadSize = extraConfig->lastPLSize;
}

void Display::uiLockUnLock(bool isLogging){
    for(uint8_t i = 0; i < ESTIM_NUM; i++)
        estims[i]->uiLockUnLock(isLogging);
    // --- sliders ---
    ui->hSTrigHor->setEnabled(!isLogging);
    ui->vSTrigVert->setEnabled(!isLogging);
    ui->vSVertPos->setEnabled(!isLogging);
    // --- config tab ---
    if(isLogging){ // lock
        ui->dialVoltDiv->setEnabled(false);
        for(int i = 0; i < ui->vLChannConf->count(); i++){
            QLayoutItem *loutWidget = ui->vLChannConf->itemAt(i);
            if(loutWidget){
                QWidget *w = static_cast<QWidget*>(loutWidget->widget());
                if(w) w->setEnabled(false);
            }
        }
    }
    else{ // unlock
        ui->cBChannel->setEnabled(true);
        ui->cBOnOff->setEnabled(true);
        emit signChannelStateChanged();
    }
}

void Display::calcHTrigOnStart(ushort hTrPos, int plSz){
    pseudoHTPos = (hTrPos / 100.0f) * plSz;
}
