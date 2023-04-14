#ifndef LOGGER_H
#define LOGGER_H

#include <QDialog>
#include <QTimer>
#include <QDoubleValidator>
#include <QJsonObject>

#include "techlogger.h"

namespace Ui {class Logger;}

class Logger : public QDialog{
    Q_OBJECT

public:
    explicit Logger(QWidget *parent = nullptr);
    ~Logger();

    const QString separ = "\n\n";
    bool isLogging = false;
    void receiveAndSaveLine(const QString& logLine);

    QJsonObject toJsonObject();
    void uiFromJson(const QJsonObject& jUi);

private:
    Ui::Logger *ui;

    QTimer* timer = nullptr;
    uint8_t indicCount = 9;

    TechLogger tLogger;
    std::thread thrLog;
    QString commonDirPath;
    std::mutex mutLoc;
    std::condition_variable condVar;

    QDoubleValidator* valTroom  = nullptr;
    QDoubleValidator* valHum    = nullptr;
    QDoubleValidator* valTenv   = nullptr;
    QDoubleValidator* valCond   = nullptr;

    const QString keyTRoom  = "temp_room";
    const QString keyHumid  = "humid";
    const QString keyTEnv   = "temp_env";
    const QString keyCond   = "conduct";
    const QString keyComm   = "comment";

signals:
    void signLoggerWork(bool isOn);

private slots:
    void slotTimerTick();
    void slotAddSymb();
    void slotChooseDir();
    void slotOnOff(bool isChecked);

    void slotInputChanged(const QString &newTxt);    
};

#endif // LOGGER_H
