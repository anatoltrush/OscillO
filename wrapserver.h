#ifndef WRAPSERVER_H
#define WRAPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>

class WrapServer : public QWidget{
    Q_OBJECT

public:
    explicit WrapServer(QWidget *parent = nullptr);

    QTcpServer* tcpServer = nullptr;
    QTcpSocket* tcpSocket = nullptr;
};

#endif // WRAPSERVER_H
