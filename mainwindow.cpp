#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::slotTimerTick);
    timer->start(1000);

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

    // --- put config ---
    for (uint8_t i = 0; i < HANTEK_NUM; i++){ // Insert configs + bufLen
        hanteks[i] = new Hantek(this);
        ui->vLayout->insertWidget(ui->vLayout->count() - 1, hanteks[i]->config);
        connect(hanteks[i]->config, &Config::signLenChanged, wrapServer, &WrapServer::slotCalcWaitSize);
    }

    // --- put displays ---
    for (uint8_t i = 0; i < HANTEK_NUM; i++){ // Insert slider, displays
        QHBoxLayout *hHantekLayout = new QHBoxLayout;
        hHantekLayout->addWidget(hanteks[i]->slider);
        hHantekLayout->addWidget(hanteks[i]->lLinPerc);
        ui->gLHantek->addLayout(hHantekLayout, 0, i);
        // ---
        for (uint8_t j = 0; j < MAX_CH_NUM; j++)
            ui->gLHantek->addWidget(hanteks[i]->displays[j], j + 1, i);
    }

    // --- put player ---
    plider = new QSlider(this);
    plider->setOrientation(Qt::Horizontal);
    plider->setMinimum(0);
    plider->setMaximum(0);
    // ---
    for (uint8_t i = 0; i < 4; i++){
        lPlFrms[i] = new QLabel("0");
        QFont font = lPlFrms[i]->font();
        font.setPointSize(9); // font size
        lPlFrms[i]->setFont(font);
    }
    lPlFrms[2]->setText("/");
    // ---
    for (uint8_t i = 0; i < 4; i++){
        pBPlBtns[i] = new QPushButton("||");
        pBPlBtns[i]->setMaximumSize(60, 20);
    }
    pBPlBtns[0]->setText("|<");
    pBPlBtns[2]->setText(">");
    pBPlBtns[3]->setText(">|");
    // ---
    ui->hLPlayer->addWidget(lPlFrms[0]);
    ui->hLPlayer->addWidget(plider);
    ui->hLPlayer->addWidget(lPlFrms[1]);
    ui->hLPlayer->addWidget(lPlFrms[2]);
    ui->hLPlayer->addWidget(lPlFrms[3]);
    ui->hLPlayer->addWidget(pBPlBtns[0]);
    ui->hLPlayer->addWidget(pBPlBtns[1]);
    ui->hLPlayer->addWidget(pBPlBtns[2]);
    ui->hLPlayer->addWidget(pBPlBtns[3]);

    // --- connections ---
    connect(ui->pBInfo, SIGNAL(clicked()), wInfo, SLOT(show()));

    connect(ui->pBLogger, SIGNAL(clicked()), wLogger, SLOT(show()));
    connect(wLogger, SIGNAL(signLoggerWork(bool)), this, SLOT(slotUiLockUnLock(bool)));

    connect(ui->pBPlayer, SIGNAL(clicked()), wPlayer, SLOT(show())); // ->
    connect(pBPlBtns[0], SIGNAL(clicked()), wPlayer, SLOT(slotOneBack())); // ->
    connect(pBPlBtns[1], SIGNAL(clicked()), wPlayer, SLOT(slotPause())); // ->
    connect(pBPlBtns[2], SIGNAL(clicked()), wPlayer, SLOT(slotPlay())); // ->
    connect(pBPlBtns[3], SIGNAL(clicked()), wPlayer, SLOT(slotOneForw())); // ->
    connect(plider, SIGNAL(valueChanged(int)), wPlayer, SLOT(slotRoughRewind(int))); // ->

    connect(wPlayer, &Player::signFrameMessage, this, &MainWindow::slotRcvFrame); // <-
    connect(wPlayer, &Player::signStateMain, this, &MainWindow::slotStateMain); // <-
    connect(wPlayer, &Player::signStatePlay, this, &MainWindow::slotStatePlay); // <-


    // --- load state ---
    loadUiState(wrapJson->getMeasConfig());
}

MainWindow::~MainWindow(){
    saveUiState();

    wPlayer->isAppStopped = true;
    std::this_thread::sleep_for(std::chrono::milliseconds(wPlayer->bigLoopMs + 3));

    delete wInfo;
    delete wLogger;
    delete wPlayer;
    delete ui;
}

void MainWindow::slotTimerTick(){
    saveUiState();
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
    // --- analyze & draw ---
    analyzeAndDraw(frames);
}

void MainWindow::slotUiLockUnLock(bool isLogging){
    ui->pBPlayer->setEnabled(!isLogging);
    for (uint8_t i = 0; i < HANTEK_NUM; i++)
        hanteks[i]->uiLockUnLock(isLogging);
}

void MainWindow::slotStatePlay(int param, int val){
    if(param == PlSt::Start){
        lPlFrms[0]->setText(QString::number(0));
        lPlFrms[1]->setText(QString::number(val));
        lPlFrms[3]->setText(QString::number(val));
        plider->setValue(0);
        plider->setMaximum(val);
        // --- ui ---
        pBPlBtns[0]->setEnabled(true);
        pBPlBtns[1]->setEnabled(false);
        pBPlBtns[2]->setEnabled(true);
        pBPlBtns[3]->setEnabled(true);
    }
    if(param == PlSt::Back || param == PlSt::Forw){
        lPlFrms[0]->setText(QString::number(val));
        lPlFrms[1]->setText(QString::number(lPlFrms[3]->text().toInt() - lPlFrms[0]->text().toInt()));
        plider->setValue(val);
    }
    if(param == PlSt::Pause){
        pBPlBtns[0]->setEnabled(true);
        pBPlBtns[1]->setEnabled(false);
        pBPlBtns[2]->setEnabled(true);
        pBPlBtns[3]->setEnabled(true);
        plider->setEnabled(true);
        lPlFrms[0]->setText(QString::number(val));
        lPlFrms[1]->setText(QString::number(lPlFrms[3]->text().toInt() - lPlFrms[0]->text().toInt()));
    }
    if(param == PlSt::Play){
        lPlFrms[0]->setText(QString::number(val));
        lPlFrms[1]->setText(QString::number(lPlFrms[3]->text().toInt() - lPlFrms[0]->text().toInt()));
        plider->setValue(val);
        // --- ui ---
        pBPlBtns[0]->setEnabled(false);
        pBPlBtns[1]->setEnabled(true);
        pBPlBtns[2]->setEnabled(false);
        pBPlBtns[3]->setEnabled(false);
        plider->setEnabled(false);
    }
}

void MainWindow::loadUiState(const QJsonObject jFull){
    if(jFull.isEmpty()){
        QMessageBox::critical(this, "Error", "Bad json input data");
        return;
    }
    // ---
    QJsonArray hants = jFull[keyHanteks].toArray();
    if(hants.size() != HANTEK_NUM){
        QMessageBox::critical(this, "Error", "Bad json input data. NofHntks != 2");
        return;
    }
    // --- hanteks ---
    for (uint8_t i = 0; i < HANTEK_NUM; i++)
        hanteks[i]->uiFromJson(hants[i].toObject());
    // --- logger ---
    QJsonObject jLog = jFull[keyLogger].toObject();
    wLogger->uiFromJson(jLog);
}

QJsonObject MainWindow::collectJson(){
    QJsonObject jFull;

    QJsonArray jHanteks;
    for (uint8_t i = 0; i < HANTEK_NUM; i++)
        jHanteks.push_back(hanteks[i]->toJsonObject());
    jFull[keyHanteks] = jHanteks;

    jFull[keyLogger] = wLogger->toJsonObject();

    return jFull;
}

void MainWindow::saveUiState(){
    QJsonObject newState = collectJson();
    if(newState != currState){
        wrapJson->saveMeasConfig(newState);
        currState = newState;
    }
}

void MainWindow::analyzeAndDraw(const std::vector<Frame> &frames){
    bool isOne = frames.size() == 1;
    for(const auto& frame : frames)
        for(uint8_t i = 0; i < MAX_CH_NUM; i++)
            if(frame.deviceIndex == i){
                hanteks[i]->rcvAndAnalyze(frame);
                hanteks[i]->rcvAndDraw(frame, isOne);
            }
}
