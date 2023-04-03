#ifndef WRAPJSON_H
#define WRAPJSON_H

#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QTimer>
#include <QApplication>

class WrapJson : public QWidget{
    Q_OBJECT

public:
    explicit WrapJson(QWidget *parent = nullptr);

private:
    const QString strConn = "configuration_connection.json";
    const QString strMeas = "configuration_measurement.json";

    void loadConnect();
    void loadMeasure();
};

#endif // WRAPJSON_H
