#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "hantek.h"
#include "wrapserver.h"
#include "wrapjson.h"
// --- windows ---
#include "info.h"
#include "logger.h"
#include "player.h"

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
    Player* wPlayer = nullptr;

    void drawChart(const std::vector<Frame>& frames);
};
#endif // MAINWINDOW_H

// TODO: load json + UI
// TODO: Logger
// TODO: UI estim
// BUG: overlap
