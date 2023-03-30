#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    // --- UI ---
    int win_wid = this->geometry().width();
    float aspRatPanels = 0.02f; // NOTE: config side panels width
    ui->splitter->setSizes({(int)(win_wid * (1.0f - aspRatPanels)), (int)(win_wid * aspRatPanels)});

    for (uint8_t i = 0; i < HANTEK_NUM; i++)
        hanteks[i] = new Hantek(this);
    for (uint8_t i = 0; i < HANTEK_DISPL_NUM; i++)
        ui->gLHantek0->addWidget(hanteks[0]->displays[i]);
    for (uint8_t i = 0; i < HANTEK_DISPL_NUM; i++)
        ui->gLHantek1->addWidget(hanteks[1]->displays[i]);
    // ---
}

MainWindow::~MainWindow(){
    delete ui;
}

