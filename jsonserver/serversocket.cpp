#include "serversocket.h"
#include <QtNetwork>
#include <QDebug>
serversocket::serversocket(QObject *parent) :
    QTcpServer(parent)
{
   listen(QHostAddress::Any, 8888);
    qDebug() << serverAddress() << serverPort() << serverError();
}

serversocket::~serversocket()
{
close();
}
void serversocket::incomingConnection(int socketDescriptor,QString location)
{
    qDebug() << "File transfer started";

    sendthread *thread = new sendthread(socketDescriptor,location,this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
    qDebug() << "Thread called";
}
