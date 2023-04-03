#ifndef WRAPJSON_H
#define WRAPJSON_H

#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QTimer>
#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>

class WrapJson : public QWidget{
    Q_OBJECT

public:
    explicit WrapJson(QWidget *parent = nullptr);

    uint64_t getPort(){return jsonInpConnect["port"].toInt();}

private:
    const QString strConn = "configuration_connection.json";
    const QString strMeas = "configuration_measurement.json";

    QJsonObject jsonInpConnect;
    QJsonObject jsonInpMeasure;

    void loadConnect();
    void loadMeasure();
};

#endif // WRAPJSON_H
