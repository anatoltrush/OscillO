#ifndef CONFIG_H
#define CONFIG_H

#include <QWidget>
#include <QIntValidator>

#include "entity.h"

namespace Ui {class Config;}

/**
 * @brief Another visual part of Hantek class
 * @details Contains some hantek configs
 */
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
    QIntValidator* valPulseWid = nullptr;

signals:
    void signLenChanged(uint64_t bufLen);

private slots:
    void slotFormat(int ind){extraConfig->m_nYTFormat = ind;}
    void slotTimDiv(int ind){controlData->nTimeDIV = ind;}
    void slotTrigMode(int ind);
    void slotTrigSweep(int ind){extraConfig->m_nTriggerSweep = ind;}
    void slotTrigSrc(int ind){controlData->nTriggerSource = ind;}
    void slotTrigSlope(int ind){controlData->nTriggerSlope = ind;}
    void slotBufLen(const QString& strLen);

    void slotPWCond(int ind){extraConfig->pwCondInd = ind;}
    void slotPulseUnit(int ind){extraConfig->pulseWidUnit = ind;}
    void slotInpPulseWid();
};

#endif // CONFIG_H
