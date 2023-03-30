#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    // --- UI ---
    int win_wid = this->geometry().width();
    float aspRatPanels = 0.1f; // NOTE: config side panel width
    ui->splitter->setSizes({(int)(win_wid * (1.0f - aspRatPanels)), (int)(win_wid * aspRatPanels)});

    for (uint8_t i = 0; i < HANTEK_NUM; i++){
        hanteks[i] = new Hantek(this);
        ui->vLayout->insertWidget(ui->vLayout->count() - 1, hanteks[i]->config);
    }

    for (uint8_t i = 0; i < HANTEK_NUM; i++)
        for (uint8_t j = 0; j < MAX_CH_NUM; j++)
            ui->gLHantek->addWidget(hanteks[i]->displays[j], j + 1, i);
    // ---
}

MainWindow::~MainWindow(){
    delete ui;
}

