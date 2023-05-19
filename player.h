#ifndef PLAYER_H
#define PLAYER_H

#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaType>

#include <fstream>
#include <thread>

#include "entity.h"

namespace Ui {class Player;}

Q_DECLARE_METATYPE(std::vector<Frame>)

class Player : public QDialog{
    Q_OBJECT

public:
    explicit Player(QWidget *parent = nullptr);
    ~Player();

    bool isAppStopped   = false;
    uint16_t bigLoopMs  = 50;

private:
    Ui::Player *ui;

    bool isPlay         = false;

    uint16_t smallLoopMcs = 0;

    QString pathFileLog;
    std::thread thrPlayFile;

    uint64_t currFrameIndex = 0;
    std::vector<Frame> frames;

    std::vector<QString> readStrings(QFile &file, int linesAmount);
    void makeFrames(const std::vector<QString>& strings);
    void playLogFile();

signals:    
    void signFrameMessage(const std::vector<Frame>& frames);
    void signStateMain(const QJsonObject& jState);
    void signStatePlay(int param, int val);

public slots:
    void slotOneBack();
    void slotPause();
    void slotPlay(){isPlay = true;}
    void slotOneForw();

private slots:
    void slotChooseFile();
    void slotPathChanged(const QString& newStr){pathFileLog = newStr;}
    void slotLoadFile();
};

#endif // PLAYER_H
