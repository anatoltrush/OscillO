#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    wInfo = new Info(this);
    wLogger = new Logger(this);
    wPlayer = new Player(this);
    wrapJson = new WrapJson(this);
    wrapServer = new WrapServer(this);

    connect(wrapServer, &WrapServer::signStringMessage, wInfo, &Info::slotGetString);
    connect(wrapServer, &WrapServer::signFrameMessage, this, &MainWindow::slotRcvFrame);
    wrapServer->start(wrapJson->getPort());

    // --- UI ---
    int win_wid = this->geometry().width();
    float aspRatPanels = 0.02f; // NOTE: config side panel width
    ui->splitter->setSizes({(int)(win_wid * (1.0f - aspRatPanels)), (int)(win_wid * aspRatPanels)});

    for (uint8_t i = 0; i < HANTEK_NUM; i++){ // Insert configs
        hanteks[i] = new Hantek(this);
        ui->vLayout->insertWidget(ui->vLayout->count() - 1, hanteks[i]->config);
    }

    for (uint8_t i = 0; i < HANTEK_NUM; i++){ // Insert slider, displays
        QHBoxLayout *hHantekLayout = new QHBoxLayout;
        hHantekLayout->addWidget(hanteks[i]->slider);
        hHantekLayout->addWidget(hanteks[i]->lLinPerc);
        ui->gLHantek->addLayout(hHantekLayout, 0, i);
        // ---
        for (uint8_t j = 0; j < MAX_CH_NUM; j++)
            ui->gLHantek->addWidget(hanteks[i]->displays[j], j + 1, i);
    }

    // --- connections ---    
    connect(ui->pBInfo, SIGNAL(clicked()), wInfo, SLOT(show()));
    connect(ui->pBLogger, SIGNAL(clicked()), wLogger, SLOT(show()));
    connect(wLogger, SIGNAL(signLoggerWork(bool)), this, SLOT(slotUiLockUnLock(bool)));
    connect(ui->pBPlayer, SIGNAL(clicked()), wPlayer, SLOT(show()));

    // --- load state ---
    loadUiState(wrapJson->getMeasConfig());
}

MainWindow::~MainWindow(){
    saveUiState();

    delete wInfo;
    delete wLogger;
    delete wPlayer;
    delete ui;
}

void MainWindow::slotRcvFrame(const std::vector<Frame> &frames){
    if(wLogger->isLogging){
        if(!isCollectedJson){
            QJsonObject jHeader = collectJson();
            isCollectedJson = true;
            // ---
            QJsonDocument docHead(jHeader);
            QString oneLineHead(docHead.toJson(QJsonDocument::Compact));
            oneLineHead.append(wLogger->separ);
            wLogger->receiveAndSaveLine(oneLineHead);
        }
        QString bigOneLine;
        for(const auto& fr : frames)
            bigOneLine.push_back(fr.getOneQstrLine());
        wLogger->receiveAndSaveLine(bigOneLine);
    }
    else{
        isCollectedJson = false;
    }
    // ---
    drawChart(frames);
}

void MainWindow::slotUiLockUnLock(bool isLogging){
    ui->pBPlayer->setEnabled(!isLogging);
    for (uint8_t i = 0; i < HANTEK_NUM; i++)
            hanteks[i]->uiLockUnLock(isLogging);
}

void MainWindow::loadUiState(const QJsonObject jMeas){
    if(jMeas.isEmpty()){
        QMessageBox::critical(this, "Error", "Bad json input data");
        return;
    }
    // ---
    QJsonArray hants = jMeas[keyHanteks].toArray();
    if(hants.size() != HANTEK_NUM){
        QMessageBox::critical(this, "Error", "Bad json input data. NofHntks != 2");
        return;
    }
    // ---
    for (uint8_t i = 0; i < HANTEK_NUM; i++)
        hanteks[i]->uiFromJson(hants[i].toObject());
}

QJsonObject MainWindow::collectJson(){
    QJsonObject jMeas;
    QJsonArray jHanteks;
    for (uint8_t i = 0; i < HANTEK_NUM; i++)
        jHanteks.push_back(hanteks[i]->toJsonObject());
    jMeas[keyHanteks] = jHanteks;

    return jMeas; // TODO: add sensors...
}

void MainWindow::drawChart(const std::vector<Frame> &frames){
    for(const auto& frame : frames)
        for(uint8_t i = 0; i < MAX_CH_NUM; i++)
            if(frame.deviceIndex == i)
                hanteks[i]->rcvAndDraw(frame);
}
