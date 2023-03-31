#include "hantek.h"

Hantek::Hantek(QWidget *parent) : QWidget(parent){
    setGeometry(0, 0, 5, 5);

    // --- create ---
    config = new Config(this);
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
        connect(displays[i], SIGNAL(signChannelStateChanged()), this, SLOT(slotUpdAllDisplays()));

    // --- final actions ---
    slotUpdAllDisplays();
}

Hantek::~Hantek(){
}

void Hantek::slotUpdAllDisplays(){
    for(uint8_t i = 0; i < MAX_CH_NUM; i++)
        displays[i]->slotUpdateUiState();
}
