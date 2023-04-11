#include "display.h"
#include "ui_display.h"

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
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->gLChart->addWidget(chartView, 1, 1);

    // --- connections ---
    connect(ui->cBOnOff, SIGNAL(clicked(bool)), this, SLOT(slotChannOnOff(bool)));
    connect(ui->cBChannel, SIGNAL(currentIndexChanged(int)), this, SLOT(slotUpdateUiChannel()));
    connect(ui->cBVoltDiv, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChannTimDiv(int)));
    connect(ui->cBCoupling, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChannCoupl(int)));
    connect(ui->cBMult, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChannMult(int)));

    // --- final actions ---
    ui->vSVertPos->setValue(ui->vSVertPos->maximum() / 2);
    ui->vSTrigVert->setValue(ui->vSTrigVert->maximum() / 2);
    ui->hSTrigHor->setValue(ui->hSTrigHor->maximum() / 2);

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
    // --- set curr ---
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
}

void Display::updateUiLinear(float perc){
    linerPos = payLoadSize * perc;
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

void Display::showInChart(const Frame &frame){
    uint8_t maxVert = std::numeric_limits<uint8_t>::max();
    payLoadSize = frame.payload.size() - IND_TO_NUM;
    // --- axis X ---
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, frame.payload.size() - IND_TO_NUM);
    axisX->setLabelFormat("%d");
    axisX->setMinorTickCount(1);
    chart->setAxisX(axisX);
    // --- axis Y ---
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, maxVert);
    axisY->setLabelFormat("%d");
    axisY->setMinorTickCount(1);
    chart->setAxisY(axisY);

    chart->removeAllSeries();
    QLineSeries* series = new QLineSeries();
    for (size_t j = 0; j < frame.payload.size(); j++)
        series->append(j, frame.payload[j]);
    chart->addSeries(series);
    // ---
    QLineSeries* serLine = new QLineSeries();

    serLine->append(linerPos, 0);
    serLine->append(linerPos, maxVert);

    chart->addSeries(serLine);
    serLine->attachAxis(axisX);
    serLine->attachAxis(axisY);
    // ---
    series->attachAxis(axisX);
    series->attachAxis(axisY);
}

QJsonObject Display::toJsonObject(){
    QJsonObject jDisplay;
    return jDisplay;
}
