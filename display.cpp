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
    connect(ui->cBChannel, SIGNAL(currentIndexChanged(int)), this, SLOT(slotUpdateUiState()));
    connect(ui->cBVoltDiv, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChannTimDiv(int)));
    connect(ui->cBCoupling, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChannCoupl(int)));
    connect(ui->cBMult, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChannMult(int)));

    // --- final actions ---
    slotShowInChart(); // FIXME: delete later
}

Display::~Display(){
    delete ui;
}

void Display::slotUpdateUiState(){
    uint8_t currChannInd = ui->cBChannel->currentIndex();
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
    }
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

void Display::slotShowInChart(){
    // --- axis X ---
    QValueAxis *axisX = new QValueAxis;
    axisX->setLabelFormat("%d");
    axisX->setMinorTickCount(1);
    chart->setAxisX(axisX);
    // --- axis Y ---
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, 256);
    axisY->setLabelFormat("%d");
    axisY->setMinorTickCount(1);
    chart->setAxisY(axisY);

    chart->removeAllSeries();
    QLineSeries* series = new QLineSeries();
    /*for (size_t j = 0; j < intermedFrame[i].payload.size(); j++)
        series->append(j, intermedFrame[i].payload[j]);*/
    chart->addSeries(series);
    // ---
    series->attachAxis(axisX);
    series->attachAxis(axisY);
}
