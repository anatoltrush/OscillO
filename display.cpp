#include "display.h"
#include "ui_display.h"

Display::Display(QWidget *parent) : QWidget(parent), ui(new Ui::Display){
    ui->setupUi(this);

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
}

Display::~Display(){
    delete ui;
}
