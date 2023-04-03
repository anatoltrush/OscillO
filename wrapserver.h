#ifndef WRAPSERVER_H
#define WRAPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QTime>

#define GET_CUR_TIME_MILLI (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count())

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

signals:
    void signMessage(QString mess);

private slots:
    void slotNewConnection();
    void slotReadyRead();
    void slotClientDisconnected();
    void slotError(QAbstractSocket::SocketError err);
};

#endif // WRAPSERVER_H
