#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "hantek.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QTcpServer* tcpServer;
    QTcpSocket* tcpSocket;

    Hantek* hanteks[HANTEK_NUM];
};
#endif // MAINWINDOW_H
