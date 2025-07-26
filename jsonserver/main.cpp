#include <QApplication>
#include "mainwindow.h" //CentralSystem Server
#include "mainwindow2.h" //CentralSystem Client
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Central System server's Object
    MainWindow *server = new MainWindow();
    server->setWindowTitle("CENTRAL SYSTEM");
    server->show();
     //Central System Client's Object
    MainWindow2 *server2 = new MainWindow2();
    server2->setWindowTitle("CENTRAL SYSTEM");
    server2->show();
    return a.exec();
}
