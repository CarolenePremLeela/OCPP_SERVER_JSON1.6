#ifndef SENDTHREAD_H
#define SENDTHREAD_H
#include <QThread>
#include <QFile>
#include <QTcpSocket>

class sendthread :public QThread
{
public:
    sendthread(int socketdescriptor, QString Location, QObject *parent);
    void run();
    QString FILENAME;
private:
    int socketDescriptor;
};

#endif // SENDTHREAD_H
