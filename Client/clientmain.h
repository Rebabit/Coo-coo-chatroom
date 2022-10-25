#ifndef CLIENTMAIN_H
#define CLIENTMAIN_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>

#include "userlogin.h"
#include "userregister.h"
#include "clientdatacenter.h"

class ClientMain : public QObject
{
    Q_OBJECT
public:
    static ClientMain& Singleton(QString IPAddress="", int port=8888, QObject * parent = nullptr) {
        static ClientMain * singleton = new ClientMain(IPAddress, port, parent);
        return * singleton;
    }

    // start client

    void operator() (void);
    //发送功能实现
    void send(QJsonObject data);
    void send1(QString data);
    //接收并打印的槽函数

    void receiveMessage();
    void processMethod(QJsonObject data);
    bool isConnected() { return is_connected; }
    void createmainwindow(QJsonObject data);
    void MessageFromMainwindow(const QString & sendername, const QString &text);
    void MessageFromMainwindow1(QString sendername, QString text);
    void MessageFromMainwindow2(QString sendername, QJsonObject text);
    void receiveJsonObject(QJsonObject);
    void SendSearchInfoToSever(QString msg);
    void SendNewFriendInfoToSever(Kuang* This);
    void sendtouxiangmessage(QString username1, QString filename);
    void Signaturesend(QJsonObject data);

    //gjy
    void NewFriendRequest(QJsonObject data);
    void AddFriendResult(QJsonObject data);

signals:
    void serverConnected();
    void serverDisconnected();
    void UserDataReceived(QJsonObject data);
    void LogInConfirmReceived(QJsonObject data);
    void SessionMessageReceived(QJsonObject data);
    void RegistConfirmReceived(QJsonObject data);
    void AddSessionToDatabase(QJsonObject data);
    void SearchInfoReceived(QJsonObject data);

    //gjy
    void ShowNewFriendRequest(QJsonObject data);
    void ShowAddFriendResult(QJsonObject data);

private:
    ClientMain(QString IPAddress, int portOpen, QObject *parent = nullptr);
    QTcpSocket *socket;
    bool is_connected = false;
    void connectToServer();
    QString ipAdd;
    int port;
    UserLogin * login = nullptr;
    UserRegister * regist = nullptr;
    MainWindow * main = nullptr;
    ClientDataCenter & datacenter = ClientDataCenter::Singleton();
};

#endif // CLIENTMAIN_H
