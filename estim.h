#ifndef ESTIM_H
#define ESTIM_H

#include <QWidget>
#include <QComboBox>
#include <QJsonObject>

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

    QJsonObject toJsonObject();
    void uiFromJson(const QJsonObject& jUi);
    void uiLockUnLock(bool isLogging);
    double getRefVal();

private:
    Ui::Estim *ui;

    const QString keyIsAct  = "is_active";
    const QString keyType   = "type";
    const QString keyMult   = "mult";
    const QString keyRef    = "ref";
    const QString keySuff   = "suffix";
    const QString keyKoeff  = "koeff";

    QDoubleValidator* valMult = nullptr;
    QDoubleValidator* valRef = nullptr;
    QDoubleValidator* valKoeff = nullptr;

private slots:
    void slotIsActive(bool isAct);
    void slotParamChanged(int ind);
    void slotSuffChanged(int ind);
    void slotInputChanged(const QString &newTxt);

};

#endif // ESTIM_H
