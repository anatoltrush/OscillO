#ifndef TECHLOGGER_H
#define TECHLOGGER_H

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QDateTime>

#include <chrono>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <mutex>
#include <thread>
#include <condition_variable>

class TechLogger{
public:
    QString strDirPath;
    QString fileName;

    // DATA
    std::vector<std::string> data;

    bool isRunning = true;
    std::mutex mutLocal;

    void dropData(std::condition_variable* cond);
    void updFileName(const QString& name);

    QString getFullName(){return strDirPath + fileName + ".log";}
};
#endif // TECHLOGGER_H