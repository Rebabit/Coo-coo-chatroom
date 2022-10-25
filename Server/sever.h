#ifndef SEVER_H
#define SEVER_H

#include <QObject>
#include<QTcpServer>
#include<QTcpSocket>
#include<QMap>
#include<QJsonObject>
class Sever : public QTcpServer
{
    Q_OBJECT
public:
    explicit Sever(QObject *parent = nullptr);

    //handle索引的socket
    QMap<int, QTcpSocket*> clientMap;

    //username索引的handle，实现了一对多
    QMap<QString, QList<int>> userToHandle;

private:
    QTcpSocket *sock;
    void incomingConnection(qintptr handle);
    void receiveMessage(qintptr);
    QString messageSender = "";

public slots:
    void setIP(QString);


signals:
     void linkMsg(QString);
     void sendMsg(QList<QJsonObject>, int);//将tcp_server收到的信息作为信号发送给 MainWindow
     void ready_Read(qintptr);
     void sendChannel(int);
     void offLine(int);

};

#endif // SEVER_H
