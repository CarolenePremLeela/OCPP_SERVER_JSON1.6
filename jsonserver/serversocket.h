#ifndef serversocket_H
#define serversocket_H

#include <QMainWindow>
#include <QDebug>
#include <QTcpServer>
#include "sendthread.h"


class serversocket : public QTcpServer
{
    Q_OBJECT

public:
    explicit serversocket(QObject *parent = 0);
    QString location;
    void ServerListener();
    ~serversocket();
    void incomingConnection(int socketDescriptor, QString location);
};

#endif // serversocket_H
