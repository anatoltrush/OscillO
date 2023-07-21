#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "hantek.h"
#include "wrapserver.h"
#include "wrapjson.h"
// --- windows ---
#include "info.h"
#include "logger.h"
#include "player.h"

//! @author Anatoly Trush @link https://www.linkedin.com/in/anatol-trush-1703b0133/

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief The MainWindow class. Main class =)
 * @details Receives frames from server and player
 * Further sends them to show in UI (and in logger if needed)
 */
class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void slotTimerTick();
    void slotRcvFrame(const std::vector<Frame>& frames);
    void slotUiLockUnLock(bool isLogging);
    void slotStateMain(const QJsonObject& jState){loadUiState(jState);}
    void slotStatePlay(int param, int val);

private:
    Ui::MainWindow *ui;
    QTimer* timer = nullptr;

    WrapServer* wrapServer = nullptr;
    WrapJson* wrapJson = nullptr;
    QJsonObject currState;

    const QString keyHanteks = "HANTEKS";
    const QString keyLogger = "Logger";
    Hantek* hanteks[HANTEK_NUM]; //! 2 Hanteks

    Info* wInfo = nullptr; //! window-Info
    Logger* wLogger = nullptr; //! window-Logger
    Player* wPlayer = nullptr; //! window-Player

    // --- player ---
    QSlider* plider = nullptr;
    QLabel* lPlFrms[4]; //! Data about frames in Player (0-shown, 1-left, 2-sep"/", 3-all)
    QPushButton* pBPlBtns[4]; //! Player buttons (0-1back, 1-pause, 2-play, 3-1forw)

    bool isCollectedJson = false;
    void loadUiState(const QJsonObject jFull);
    QJsonObject collectJson();
    void saveUiState();
    void analyzeAndDraw(const std::vector<Frame>& frames);
};
#endif // MAINWINDOW_H

// TODO: ? Marker
