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
    uint64_t waitSize = 0;

    QTime time;
    QByteArray buffArray;

    void parseAndSendData(QByteArray &array);
    bool isAccumulated(const QByteArray& array);

signals:
    void signStringMessage(QString mess);
    void signFrameMessage(const std::vector<Frame>& frames);

public slots:
    void slotCalcWaitSize(uint64_t bufLen){waitSize = (bufLen * 2 + 1) * MAX_CH_NUM + 2;}

private slots:
    void slotNewConnection();
    void slotReadyRead();
    void slotClientDisconnected();
    void slotError(QAbstractSocket::SocketError err);
};

#endif // WRAPSERVER_H
