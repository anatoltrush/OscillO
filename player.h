#ifndef PLAYER_H
#define PLAYER_H

#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>

#include <fstream>

#include "entity.h"

namespace Ui {class Player;}

class Player : public QDialog{
    Q_OBJECT

public:
    explicit Player(QWidget *parent = nullptr);
    ~Player();

public slots:
    void slotOneBack();
    void slotPause(){isPaused = !isPaused;}
    void slotPlay();
    void slotOneForw();

private:
    Ui::Player *ui;

    QString pathFileLog;
    bool isPaused = false;
    std::vector<Frame> frames;

    std::vector<QString> readStrings(QFile &file, int linesAmount);
    void makeFrames(const std::vector<QString>& strings);

private slots:
    void slotChooseFile();
    void slotPathChanged(const QString& newStr){pathFileLog = newStr;}
    void slotLoadFile();
};

#endif // PLAYER_H
