#include "hantek.h"

uint8_t Hantek::countNum = 1;

Hantek::Hantek(QWidget *parent) : QWidget(parent){
    //setGeometry(0, 0, 1, 1);

    // --- create ---
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
    config->relayControl = &relayControl;
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
    slotUpdAllChannels();

    for(uint8_t i = 0; i < MAX_CH_NUM; i++)
        displays[i]->chooseChannel(i);
}

Hantek::~Hantek(){}

void Hantek::rcvAndDraw(const Frame &frame){
    for(uint8_t i = 0; i < MAX_CH_NUM; i++)
        if(displays[i]->currChannInd == frame.channelNum && relayControl.bCHEnable[frame.channelNum])
            displays[i]->showInChart(frame);
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
