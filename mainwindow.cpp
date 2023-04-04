#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    wInfo = new Info(this);
    wrapJson = new WrapJson(this);
    wrapServer = new WrapServer(this);

    connect(wrapServer, SIGNAL(signMessage(QString)), wInfo, SLOT(slotGetString(QString)));
    wrapServer->start(wrapJson->getPort());

    // --- UI ---
    int win_wid = this->geometry().width();
    float aspRatPanels = 0.02f; // NOTE: config side panel width
    ui->splitter->setSizes({(int)(win_wid * (1.0f - aspRatPanels)), (int)(win_wid * aspRatPanels)});

    for (uint8_t i = 0; i < HANTEK_NUM; i++){
        hanteks[i] = new Hantek(this);
        ui->vLayout->insertWidget(ui->vLayout->count() - 1, hanteks[i]->config);
    }

    for (uint8_t i = 0; i < HANTEK_NUM; i++){
        ui->gLHantek->addWidget(hanteks[i]->slider, 0, i);
        for (uint8_t j = 0; j < MAX_CH_NUM; j++)
            ui->gLHantek->addWidget(hanteks[i]->displays[j], j + 1, i);
    }

    // --- connections ---
    connect(ui->pBInfo, SIGNAL(clicked()), wInfo, SLOT(show()));
    connect(ui->pBDirSaveData, SIGNAL(clicked()), this, SLOT(slotChooseDir()));

    // --- logger ---
    tLogger.updFileName("/HANTEK_DATA_");
    tLogger.strDirPath = QDir::currentPath();
    ui->lPathSaveData->setText("Path: " + tLogger.getFullName());
    // --- threads ---
    thrLog = std::thread(&TechLogger::dropData, &tLogger, &condVar);
}

MainWindow::~MainWindow(){
    tLogger.isRunning = false;
    if(thrLog.joinable()) thrLog.join();

    delete ui;
}

void MainWindow::slotChooseDir(){
    QString tempStr = QFileDialog::getExistingDirectory(this,
                                                        tr("Save data to..."), commonDirPath,
                                                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(!tempStr.isEmpty()){
        if(tempStr.back() == '/')
            tempStr.chop(1);
        commonDirPath = tempStr;
        tLogger.strDirPath = tempStr;
        ui->lPathSaveData->setText("Path: " + tLogger.getFullName());
    }
}
