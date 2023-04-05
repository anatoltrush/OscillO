#ifndef DISPLAY_H
#define DISPLAY_H

#include <QWidget>
#include <QLineSeries>
#include <QChartView>
#include <QGraphicsLayout>
#include <QValueAxis>

#include "entity.h"

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

    uint8_t currChannInd = 0;
    uint16_t linerPos = 0;

    void chooseChannel(uint8_t ch);
    void showInChart(const Frame& frame);

private:
    Ui::Display *ui;

    QChart *chart           = nullptr;
    QChartView *chartView   = nullptr;

signals:
    void signChannelStateChanged();

public slots:
    void slotUpdateUiChannel();
    void slotUpdateUiLinear(float perc);

private slots:
    void slotChannOnOff(bool isOn);
    void slotChannTimDiv(int ind);
    void slotChannCoupl(int ind);
    void slotChannMult(int ind);
};

#endif // DISPLAY_H
