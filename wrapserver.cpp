#include "wrapserver.h"

WrapServer::WrapServer(QWidget *parent) : QWidget(parent){
    tcpServer = new QTcpServer;
}

void WrapServer::start(uint64_t port){
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
    tcpServer->listen(QHostAddress::AnyIPv4, port);
    emit signMessage("Listen port: " + QString::number(port));

    emit signMessage("<--- Server started --->");
    if(tcpServer->isListening())
        emit signMessage("...listening...");
}

void WrapServer::slotNewConnection(){
    tcpSocket = tcpServer->nextPendingConnection();
    if(!tcpSocket) return;
    connect(tcpSocket, &QTcpSocket::readyRead, this, &WrapServer::slotReadyRead);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &WrapServer::slotClientDisconnected);
    connect(tcpSocket, &QTcpSocket::errorOccurred, this, &WrapServer::slotError);

    emit signMessage("New connection detected: " + tcpSocket->objectName() + " | " + tcpSocket->localAddress().toString());
}

void WrapServer::slotReadyRead(){
    uint64_t diffMs = GET_CUR_TIME_MILLI - prevMs;
    prevMs = GET_CUR_TIME_MILLI;

    // ---
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if(!socket) return;

    QByteArray readAll = socket->readAll();

    // --- UI ---
    time = QTime::currentTime();
    emit signMessage("[" + QString::number(rcvCounter) + "] " +
                          time.toString() + " | Received data size: " +
                          QString::number(readAll.size()) + "bytes | " +
                          QString::number(diffMs) + "ms");
    // ---
    //TODO: //parseAndSaveData(readAll);
}

void WrapServer::slotClientDisconnected(){
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if(!socket) return;
    socket->close();
    socket->deleteLater();
    emit signMessage("Disconnected: " + tcpSocket->objectName() + " | " + tcpSocket->localAddress().toString());
}

void WrapServer::slotError(QAbstractSocket::SocketError err){
    QString strError =  "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                                         "The host was not found." : err == QAbstractSocket::RemoteHostClosedError ?
                                             "The remote host is closed." : err == QAbstractSocket::ConnectionRefusedError ?
                                                 "The connection was refused." :
                                                 QString(tcpSocket->errorString()) );
    emit signMessage(strError);
}
