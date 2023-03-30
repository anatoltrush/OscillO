#ifndef DISPLAY_H
#define DISPLAY_H

#include <QWidget>
#include <QLineSeries>
#include <QChartView>
#include <QGraphicsLayout>

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

private:
    Ui::Display *ui;

    QChart *chart           = nullptr;
    QChartView *chartView   = nullptr;
};

#endif // DISPLAY_H
