#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QJsonObject>
#include <QDebug>
#include <QMessageBox>
#include <QJsonDocument>

class Server: public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr): QTcpServer (parent){}
    QMap<int, QTcpSocket*> clientMap; //handle索引的socket
    QMap<QString, QList<int>> userToHandle; //username索引的handle的list，一对多
private:
    QTcpSocket *sock;
    QString messageSender = "";
    void incomingConnection(qintptr handle);//产生新链接时自动调用
    void receiveMessage(qintptr);
public slots:
    void setIP(QString);
signals:
    void linkMsg(QString);
    void sendMsg(QList<QJsonObject>, int);
    void ready_Read(qintptr);
    void sendChannel(int);
    void offLine(int);
};

#endif // SERVER_H
