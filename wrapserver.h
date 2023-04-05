#ifndef WRAPSERVER_H
#define WRAPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QTime>

#include "entity.h"

class WrapServer : public QWidget{
    Q_OBJECT

public:
    explicit WrapServer(QWidget *parent = nullptr);

    QTcpServer* tcpServer = nullptr;
    QTcpSocket* tcpSocket = nullptr;

    void start(uint64_t port);

private:
    uint64_t prevMs = 0;
    uint64_t rcvCounter = 0;

    QTime time;

    void parseAndSendData(QByteArray &array);

signals:
    void signStringMessage(QString mess);
    void signFrameMessage(const std::vector<Frame>& frames);

private slots:
    void slotNewConnection();
    void slotReadyRead();
    void slotClientDisconnected();
    void slotError(QAbstractSocket::SocketError err);
};

#endif // WRAPSERVER_H
