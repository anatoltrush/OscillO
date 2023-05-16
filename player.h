#ifndef PLAYER_H
#define PLAYER_H

#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>

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
    void slotPause();
    void slotPlay();
    void slotOneForw();

private:
    Ui::Player *ui;

    QString pathFileLog;
    std::vector<Frame> frames;

    void readData(QFile &file, int linesAmount);

private slots:
    void slotChooseFile();
    void slotLoadFile();
};

#endif // PLAYER_H
