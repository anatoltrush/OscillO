#ifndef HANTEK_H
#define HANTEK_H

#include <QWidget>

#include "entity.h"
#include "display.h"

struct HantConfig{
};

class Hantek : public QWidget{
    Q_OBJECT

public:
    explicit Hantek(QWidget *parent = nullptr);
    ~Hantek();

    HantConfig config;

    Display* displays[HANTEK_DISPL_NUM];

private:
};

#endif // HANTEK_H
