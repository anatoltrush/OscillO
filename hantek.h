#ifndef HANTEK_H
#define HANTEK_H

#include <QSlider>
#include <QLabel>

#include "display.h"
#include "config.h"

class Hantek : public QWidget{
    Q_OBJECT

public:
    explicit Hantek(QWidget *parent = nullptr);
    ~Hantek();

    const QString keyRelayControl   = "relayControl";
    const QString keyControlData    = "controlData";
    const QString keyExtraConfig    = "extraConfig";
    RELAYCONTROL relayControl;
    CONTROLDATA controlData;
    EXTRACONFIG extraConfig;

    QSlider* slider = nullptr;
    QLabel* lLinPerc = nullptr;

    Config* config = nullptr;
    Display* displays[MAX_CH_NUM];

    void rcvAndDraw(const Frame& frame);
    QJsonObject toJsonObject();

private:
    static uint8_t countNum;

private slots:
    void slotUpdAllChannels();
    void slotUpdAllLinear(int pos);
};

#endif // HANTEK_H
