#include "clientdatacenter.h"
#include "userlogin.h"
#include "userregister.h"
#include "mainwindow.h"
#include "kuang.h"
#include "clientmain.h"
#include <QApplication>
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDir>


const QString filename = QDir::currentPath() +  "/settings.json";

QJsonObject readSettings() {
    QFile file(filename);
    if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QByteArray t = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(t);
        file.close();
        qDebug() << "loaded local settings " << file.fileName() << doc.object();
        return doc.object();
    }
    return QJsonObject();
}


#define DEBUG

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifndef DEBUG
    auto settings = readSettings();
    QString ip = settings.contains("Server IP") ? settings["Server IP"].toString() : "192.168.43.125";
    int port = settings.contains("Port") ? settings["Port"].toInt() : 8888;
#else
    QString ip = "192.168.31.45";
    int port = 8888;
#endif
    qDebug() << "IP = " << ip << ", port = " << port;

    ClientMain & client = ClientMain::Singleton(ip, port);
    client();

//    MainWindow mainw;
//    mainw.show();
//    mainw.showNewFriendPop("lulala");

    return a.exec();




}
