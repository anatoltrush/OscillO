#ifndef HANTEK_H
#define HANTEK_H

#include <QSlider>

#include "display.h"
#include "config.h"

class Hantek : public QWidget{
    Q_OBJECT

public:
    explicit Hantek(QWidget *parent = nullptr);
    ~Hantek();

    RELAYCONTROL relayControl;
    CONTROLDATA controlData;
    EXTRACONFIG extraConfig;

    QSlider* slider = nullptr;
    Config* config = nullptr;
    Display* displays[MAX_CH_NUM];

private:
    static uint8_t countNum;

private slots:
    void slotUpdAllChannels();
    void slotUpdAllLinear(int pos);
};

#endif // HANTEK_H
