#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "hantek.h"
#include "info.h"
#include "wrapserver.h"
#include "wrapjson.h"
#include "techlogger.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void slotChooseDir();
    void slotRcvFrame(const std::vector<Frame>& frames);

private:
    Ui::MainWindow *ui;

    WrapServer* wrapServer = nullptr;
    WrapJson* wrapJson = nullptr;

    Hantek* hanteks[HANTEK_NUM];
    Info* wInfo = nullptr;

    TechLogger tLogger;
    std::thread thrLog;
    QString commonDirPath;
    std::mutex mutLoc;
    std::condition_variable condVar;

    void drawChart(const std::vector<Frame>& frames);
};
#endif // MAINWINDOW_H

// TODO: load json + UI (displ+conf)
