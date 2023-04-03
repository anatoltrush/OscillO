#include "wrapjson.h"

WrapJson::WrapJson(QWidget *parent) : QWidget(parent){
    loadConnect();
    loadMeasure();
}

void WrapJson::loadConnect(){
    QFile jsonFile(strConn);
    if (!jsonFile.open(QIODevice::ReadOnly)){
        QMessageBox::critical(this, "Error", "Can't find connect config file: " + strConn);
        QTimer::singleShot(0, QApplication::instance(), SLOT(quit()));
    }
}

void WrapJson::loadMeasure(){
    QFile jsonFile(strMeas);
    if (!jsonFile.open(QIODevice::ReadOnly)){
        QMessageBox::critical(this, "Error", "Can't find measure config file: " + strConn);
        QTimer::singleShot(0, QApplication::instance(), SLOT(quit()));
    }
}
