#ifndef CONFIG_H
#define CONFIG_H

#include <QWidget>

#include "entity.h"

namespace Ui {class Config;}

class Config : public QWidget{
    Q_OBJECT

public:
    explicit Config(QWidget *parent = nullptr);
    ~Config();

    CONTROLDATA* controlData = nullptr;
    EXTRACONFIG* extraConfig = nullptr;

    void setCount(uint8_t num);
    void updUIAfterModel();

    void uiLockUnLock(bool isLogging);

private:
    Ui::Config *ui;

private slots:
    void slotFormat(int ind){extraConfig->m_nYTFormat = ind;}
    void slotTimDiv(int ind){controlData->nTimeDIV = ind;}
    void slotTrigMode(int ind){extraConfig->m_nTriggerMode = ind;}
    void slotTrigSweep(int ind){extraConfig->m_nTriggerSweep = ind;}
    void slotTrigSrc(int ind){controlData->nTriggerSource = ind;}
    void slotTrigSlope(int ind){controlData->nTriggerSlope = ind;}
};

#endif // CONFIG_H

// TODO: write all combos
// BUG: 1) load HTrig, work VTrig
