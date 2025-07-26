#include "sendthread.h"
//#define FILENAME "C:/Users/firstcall/Desktop/Database/AuthorizationData.txt"
sendthread::sendthread(int socketDescriptor,QString Location,QObject *parent)
    : QThread(parent), socketDescriptor(socketDescriptor)
{
    FILENAME = Location;
}
void sendthread::run()
{
    QTcpSocket client;
    qDebug() << "Thread Descriptor :" << socketDescriptor;
    if (!client.setSocketDescriptor(socketDescriptor))
    {
        qDebug() << client.error();
        return;
    }
    qDebug() << "Thread : Connected";

    //send File
    QFile inputFile(FILENAME);
    QByteArray read;
    inputFile.open(QIODevice::ReadOnly);
    while(1)
    {
        read.clear();
        read = inputFile.read(32768*8);
        qDebug() << "Read : " << read.size();
        if(read.size()==0)
            break;

        qDebug() << "Written : " << client.write(read);
        client.waitForBytesWritten();
        read.clear();
    }
    inputFile.close();
    client.disconnectFromHost();
    client.waitForDisconnected();
    qDebug() << "Thread : File transfer completed";
}
