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

    RELAYCONTROL* relayControl = nullptr;
    CONTROLDATA* controlData = nullptr;
    EXTRACONFIG* extraConfig = nullptr;

    void setCount(uint8_t num);

private:
    Ui::Config *ui;

};

#endif // CONFIG_H
