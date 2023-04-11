#include "wrapjson.h"

WrapJson::WrapJson(QWidget *parent) : QWidget(parent){
    loadConnect();
    loadMeasure();
}

void WrapJson::saveMeasConfig(const QJsonObject &jMeas){
    QFile jsonFile(strMeas);
    if (!jsonFile.open(QIODevice::WriteOnly)){
        QMessageBox::critical(this, "Error", "Can't save output json file");
        return;
    }
    // ---

    jsonFile.write(QJsonDocument(jMeas).toJson(QJsonDocument::Indented));
    jsonFile.close();
}

void WrapJson::loadConnect(){
    QFile jsonFile(strConn);
    if (!jsonFile.open(QIODevice::ReadOnly)){
        QMessageBox::critical(this, "Error", "Can't find connect config file: " + strConn);
        QTimer::singleShot(0, QApplication::instance(), SLOT(quit()));
    }
    // ---
    QByteArray fileData = jsonFile.readAll();
    QJsonDocument jsonDocument(QJsonDocument::fromJson(fileData));
    jsonInpConnect = jsonDocument.object();
    if(jsonInpConnect.isEmpty()){
        QMessageBox::critical(this, "Error", "Bad .json file: " + strConn);
        QTimer::singleShot(0, QApplication::instance(), SLOT(quit()));
    }
    // ---
}

void WrapJson::loadMeasure(){
    QFile jsonFile(strMeas);
    if (!jsonFile.open(QIODevice::ReadOnly)){
        QMessageBox::critical(this, "Error", "Can't find measure config file: " + strMeas);
        QTimer::singleShot(0, QApplication::instance(), SLOT(quit()));
    }
    // ---
    QByteArray fileData = jsonFile.readAll();
    QJsonDocument jsonDocument(QJsonDocument::fromJson(fileData));
    jsonInpMeasure = jsonDocument.object();
    if(jsonInpMeasure.isEmpty()){
        QMessageBox::critical(this, "Error", "Bad .json file: " + strMeas);
        QTimer::singleShot(0, QApplication::instance(), SLOT(quit()));
    }
    // ---
}
