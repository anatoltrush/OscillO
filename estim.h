#ifndef ESTIM_H
#define ESTIM_H

#include <QWidget>
#include <QComboBox>

namespace Ui {
class Estim;
}

class Estim : public QWidget
{
    Q_OBJECT

public:
    explicit Estim(QWidget *parent = nullptr);
    ~Estim();

    void setParamNum(uint8_t num);

private:
    Ui::Estim *ui;

private slots:
    void slotIsActive(bool isAct);
    void slotParamChanged(int ind);

};

#endif // ESTIM_H
