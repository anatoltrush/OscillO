#include "hantek.h"

uint8_t Hantek::countNum = 1;

Hantek::Hantek(QWidget *parent) : QWidget(parent){
    slider = new QSlider(this);
    slider->setOrientation(Qt::Horizontal);

    lLinPerc = new QLabel(this);
    lLinPerc->setText("   percent: 0%   ");

    config = new Config(this);
    config->setCount(countNum);
    countNum++;

    for(uint8_t i = 0; i < MAX_CH_NUM; i++){
        displays[i] = new Display(this);
        displays[i]->show();
    }

    // --- bindings ---
    config->controlData = &controlData;
    config->extraConfig = &extraConfig;
    for(uint8_t i = 0; i < MAX_CH_NUM; i++){
        displays[i]->relayControl = &relayControl;
        displays[i]->controlData = &controlData;
        displays[i]->extraConfig = &extraConfig;
    }

    // --- connections ---
    for(uint8_t i = 0; i < MAX_CH_NUM; i++)
        connect(displays[i], SIGNAL(signChannelStateChanged()), this, SLOT(slotUpdAllChannels()));

    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(slotUpdAllLinear(int)));

    // --- final actions ---
    for(uint8_t i = 0; i < MAX_CH_NUM; i++)
        displays[i]->chooseChannel(i);
}

Hantek::~Hantek(){}

void Hantek::rcvAndDraw(const Frame &frame){
    for(uint8_t i = 0; i < MAX_CH_NUM; i++)
        if(displays[i]->currChannInd == frame.channelNum && relayControl.bCHEnable[frame.channelNum])
            displays[i]->showInChart(frame);
}

QJsonObject Hantek::toJsonObject(){
    QJsonObject jHantek;

    jHantek[keyRelayControl] = relayControl.toJsonObject();
    jHantek[keyControlData] = controlData.toJsonObject();
    jHantek[keyExtraConfig] = extraConfig.toJsonObject();

    QJsonArray jDisplays;
    for(uint8_t i = 0; i < MAX_CH_NUM; i++)
        jDisplays.push_back(displays[i]->toJsonObject());
    jHantek[keyDisplays] = jDisplays;

    return jHantek;
}

void Hantek::uiFromJson(const QJsonObject &jHantek){
    if(jHantek.isEmpty()){
        QMessageBox::critical(this, "Error", "Bad json input (Hantek data)");
        return;
    }
    // ---
    QJsonArray jArrDisps = jHantek[keyDisplays].toArray();
    if(jArrDisps.size() != MAX_CH_NUM){
        QMessageBox::critical(this, "Error", "Bad json input data. NofChnnls != 4");
        return;
    }
    // --- load model ---
    loadModel(jHantek);
    // --- load UI ---
    for(uint8_t i = 0; i < MAX_CH_NUM; i++)
        displays[i]->uiFromJson(jArrDisps[i].toObject());
    config->updUIAfterModel();
}

void Hantek::uiLockUnLock(bool isLogging){
    for(uint8_t i = 0; i < MAX_CH_NUM; i++)
        displays[i]->uiLockUnLock(isLogging);
    config->uiLockUnLock(isLogging);
}

void Hantek::loadModel(const QJsonObject &jHantek){
    QJsonObject jRelCntrl = jHantek[keyRelayControl].toObject();
    relayControl.fromJson(jRelCntrl);

    QJsonObject jCntrlData = jHantek[keyControlData].toObject();
    controlData.fromJson(jCntrlData);

    QJsonObject jExtra = jHantek[keyExtraConfig].toObject();
    extraConfig.fromJson(jExtra);
}

void Hantek::slotUpdAllChannels(){
    for(uint8_t i = 0; i < MAX_CH_NUM; i++)
        displays[i]->slotUpdateUiChannel();
}

void Hantek::slotUpdAllLinear(int pos){
    float perc = pos / (float)slider->maximum();
    for(uint8_t i = 0; i < MAX_CH_NUM; i++)
        displays[i]->updateUiLinear(perc);
    lLinPerc->setText("   percent: " + QString::number((uint8_t)(perc * 100)) + "%   ");
}
