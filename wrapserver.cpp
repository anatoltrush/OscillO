#include "wrapserver.h"

WrapServer::WrapServer(QWidget *parent) : QWidget(parent){
    tcpServer = new QTcpServer;
}

void WrapServer::start(uint64_t port){
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
    tcpServer->listen(QHostAddress::AnyIPv4, port);
    emit signStringMessage("Listen port: " + QString::number(port));

    emit signStringMessage("<--- Server started --->");
    if(tcpServer->isListening())
        emit signStringMessage("...listening...");
}

void WrapServer::parseAndSendData(QByteArray &array){
    if(array.size() < Frame::getHeaderSize()) return;
    // --- device index ---
    uint8_t devInd = array.back();
    array.chop(1);
    // --- nof channels ---
    uint8_t parts = array.back();
    if (parts == 0 || parts > MAX_CH_NUM) return;
    array.chop(1);
    // --- parts ---
    if(array.size() % 2 != 0) return;
    uint32_t partLen = array.size() / parts;
    // ---
    uint32_t tempLen = partLen - 1;
    if(tempLen % DATA_SIZE_STEP != 0)
        return;
    // ---
    std::vector<Frame> locFrames;
    for(uint8_t j = 0; j < parts; j++){
        QByteArray locBArr = array.mid(j * partLen, partLen);
        Frame localFrame;
        localFrame.payload.clear();
        localFrame.deviceIndex = devInd;
        localFrame.timeStamp = GET_CUR_TIME_MICRO;
        localFrame.channelNum = locBArr.back();
        locBArr.chop(1);
        for(int i = 0; i < locBArr.size(); i += 2){
            ushort value = 0;
            char char1 = locBArr[i];
            char char2 = locBArr[i + 1];
            ((char*)&value)[0] = char1;
            ((char*)&value)[1] = char2;
            localFrame.payload.push_back(value);
        }
        locFrames.push_back(localFrame);
    }
    // --- sending ---
    emit signFrameMessage(locFrames);
}

void WrapServer::slotNewConnection(){
    tcpSocket = tcpServer->nextPendingConnection();
    if(!tcpSocket) return;
    connect(tcpSocket, &QTcpSocket::readyRead, this, &WrapServer::slotReadyRead);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &WrapServer::slotClientDisconnected);
    connect(tcpSocket, &QTcpSocket::errorOccurred, this, &WrapServer::slotError);

    emit signStringMessage("New connection detected: " + tcpSocket->objectName() + " | " + tcpSocket->localAddress().toString());
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
    QString gotStr = "[" + QString::number(rcvCounter) + "] " +
            socket->localAddress().toString() + " | " +
            time.toString() + " | Received data size: " +
            QString::number(readAll.size()) + "bytes | " +
            QString::number(diffMs) + "ms";
    emit signStringMessage(gotStr);
    rcvCounter++;
    // ---
    parseAndSendData(readAll);
}

void WrapServer::slotClientDisconnected(){
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if(!socket) return;
    socket->close();
    socket->deleteLater();
    emit signStringMessage("Disconnected: " + socket->objectName() + " | " + tcpSocket->localAddress().toString());
}

void WrapServer::slotError(QAbstractSocket::SocketError err){
    QString strError =  "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                                         "The host was not found." : err == QAbstractSocket::RemoteHostClosedError ?
                                             "The remote host is closed." : err == QAbstractSocket::ConnectionRefusedError ?
                                                 "The connection was refused." :
                                                 QString(tcpSocket->errorString()) );
    emit signStringMessage(strError);
}
