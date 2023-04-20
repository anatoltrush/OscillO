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

    RELAYCONTROL relayControl;
    CONTROLDATA controlData;
    EXTRACONFIG extraConfig;

    QSlider* slider = nullptr;
    QLabel* lLinPerc = nullptr;

    Config* config = nullptr;
    Display* displays[MAX_CH_NUM];

    void rcvAndDraw(const Frame& frame);

    QJsonObject toJsonObject();
    void uiFromJson(const QJsonObject& jHantek);
    void uiLockUnLock(bool isLogging);

private:
    static uint8_t countNum;

    const QString keyRelayControl   = "relayControl";
    const QString keyControlData    = "controlData";
    const QString keyExtraConfig    = "extraConfig";
    const QString keyDisplays       = "displays";

    void loadModel(const QJsonObject& jHantek);

private slots:
    void slotUpdAllChannels();
    void slotUpdAllRulers(int pos);
};

#endif // HANTEK_H
