#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "hantek.h"
#include "wrapserver.h"
#include "wrapjson.h"
#include "techlogger.h"
// --- windows ---
#include "info.h"
#include "logger.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:    
    void slotRcvFrame(const std::vector<Frame>& frames);

private:
    Ui::MainWindow *ui;

    WrapServer* wrapServer = nullptr;
    WrapJson* wrapJson = nullptr;

    Hantek* hanteks[HANTEK_NUM];
    Info* wInfo = nullptr;
    Logger* wLogger = nullptr;

    TechLogger tLogger;
    std::thread thrLog;
    QString commonDirPath;
    std::mutex mutLoc;
    std::condition_variable condVar;

    void drawChart(const std::vector<Frame>& frames);
};
#endif // MAINWINDOW_H

// TODO: load json
// TODO: Validators
// TODO: Logger
// TODO: UI estim
