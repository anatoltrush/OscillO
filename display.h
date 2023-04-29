#ifndef DISPLAY_H
#define DISPLAY_H

#include <QLineSeries>
#include <QChartView>
#include <QGraphicsLayout>
#include <QValueAxis>

#include "entity.h"
#include "estim.h"
#include "analyzer.h"

#include <qmath.h>

namespace Ui {class Display;}

#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
using namespace QtCharts;
#endif

class Display : public QWidget{
    Q_OBJECT

public:
    explicit Display(QWidget *parent = nullptr);
    ~Display();

    RELAYCONTROL* relayControl = nullptr;
    CONTROLDATA* controlData = nullptr;
    EXTRACONFIG* extraConfig = nullptr;

    Estim* estims[ESTIM_NUM];

    uint8_t currChannInd = 0;
    uint16_t linerPos = 0;    

    void updateUiLinear(float perc){linerPos = extraConfig->lastPLSize * perc;}
    void chooseChannel(uint8_t ch);
    void showInChart(const Frame& frame);
    void analyze(const Frame& frame);

    QJsonObject toJsonObject();
    void uiFromJson(const QJsonObject& jUi);
    void uiLockUnLock(bool isLogging);
    static void calcHTrigOnStart(ushort hTrPos, int plSz){pseudoHTPos = (hTrPos / 100.0f) * plSz;}

private:
    Ui::Display *ui;
    static int pseudoHTPos;
    int payLoadSize = 0;

    QChart *chart           = nullptr;
    QChartView *chartView   = nullptr;    

    QColor estimColor;
    const QString keyCurrCh = "channel_show";
    const QString keyEstims = "estimations";

signals:
    void signChannelStateChanged();

public slots:
    void slotUpdateUiChannel();    

private slots:
    void slotChannOnOff(bool isOn);
    void slotChannTimDiv(int ind);
    void slotChannCoupl(int ind);
    void slotChannMult(int ind);

    void slotLevelPos(int val);
    void slotTrigVert(int val);
    void slotTrigHor(int val);
};

#endif // DISPLAY_H

 // FIXME: ? progress bar (color + other...)
