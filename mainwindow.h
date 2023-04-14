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

// TODO: 1) MSVC Hantek read .json
// TODO: (HOME) load json + UI (...+sliders)
// TODO: (HOME) Timer to save configs online
// TODO: 1) Add to Hantek path to json connection file
// TODO: (HOME) Add sensor name on display
// TODO: ? Player
// BUG: 1) Bad data receive
// BUG: overlap
