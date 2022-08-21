#include "login.h"
#include "clientsir.h"
#include <QApplication>
#include <QJsonObject>
#include <QObject>
#include <QJsonDocument>
#include <QFile>
#include <QDir>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString ip = "192.168.43.125";
    int port = 8888;
    qDebug() << "IP = " << ip << ", port = " << port;
    ClientSir & client = ClientSir::Singleton(ip, port);
    client();

    return a.exec();
}
