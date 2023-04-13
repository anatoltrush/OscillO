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
    void slotUiLockUnLock(bool isLogging);

private:
    Ui::MainWindow *ui;

    WrapServer* wrapServer = nullptr;
    WrapJson* wrapJson = nullptr;

    const QString keyHanteks = "HANTEKS";
    const QString keyLogger = "Logger";
    Hantek* hanteks[HANTEK_NUM];    

    Info* wInfo = nullptr;
    Logger* wLogger = nullptr;
    Player* wPlayer = nullptr;

    bool isCollectedJson = false;
    void loadUiState(const QJsonObject jFull);
    QJsonObject collectJson();
    void saveUiState(){wrapJson->saveMeasConfig(collectJson());}
    void drawChart(const std::vector<Frame>& frames);
};
#endif // MAINWINDOW_H

// TODO: !!! MSVC Hantek
// TODO: load json + UI
// TODO: Timer to save configs online
// TODO: Add temp, comm...(sensors) -> save&load
// TODO: Add path to json connection file
// TODO: Add sensor name on display
// BUG: !!! Bad data receive
// BUG: overlap
