#include "wrapserver.h"

WrapServer::WrapServer(QWidget *parent) : QWidget(parent){
    tcpServer = new QTcpServer;
}
