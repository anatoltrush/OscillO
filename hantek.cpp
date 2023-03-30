#include "hantek.h"

Hantek::Hantek(QWidget *parent) : QWidget(parent){
    setGeometry(0, 0, 5, 5);

    for(uint8_t i = 0; i < HANTEK_DISPL_NUM; i++){
        displays[i] = new Display(this);
        displays[i]->show();
    }    
}

Hantek::~Hantek(){
}
