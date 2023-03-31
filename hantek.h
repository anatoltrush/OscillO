#ifndef HANTEK_H
#define HANTEK_H

#include <QWidget>

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

    Config* config;
    Display* displays[MAX_CH_NUM];

private:

private slots:
    void slotUpdAllDisplays();
};

#endif // HANTEK_H
