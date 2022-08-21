#ifndef CLIENTSIR_H
#define CLIENTSIR_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>

#include "login.h"
#include "userregister.h"
#include "clientdatacenter.h"
#include "msgsession.h"

class ClientSir : public QObject
{
    Q_OBJECT
public:
    static ClientSir& Singleton(QString IPAddress = "", int port = 8899, QObject *parent = nullptr){
        static ClientSir * singleton = new ClientSir(IPAddress, port, parent);
        return *singleton;
    }
    //构造connect列表
    void operator() (void);
    //发送信息
    void send(QJsonObject data);
    //接收消息
    void receiveMessage();
    //选择对应的处理分支，即根据msgtype，发射不同的信号
    void processMethod(QJsonObject data);
    //根据返回的用户个人信息创建主界面
    void createmainwindow(QJsonObject data);
    //处理要发送的消息,打包成json并且直接发送
    void MessageFromMainwindow(const QString & sendername, const QString &text);
    //发送要查找的消息到服务器，这个功能有点类似于查找聊天记录
    void SendSearchInfoToSever(QString msg);
    //添加好友请求
    void SendNewFriendInfoToSever(MsgSession* This);


signals:
    //接收服务器返回的用户数据信号
    void UserDataReceived(QJsonObject data);
    //接收服务器返回的登录信号
    void LogInConfirmReceived(QJsonObject data);
    //接收会话信息信号
    void SessionMessageReceived(QJsonObject data);
    //接收服务器返回的注册信号
    void RegistConfirmReceived(QJsonObject data);
    //添加本地聊天记录信号
    void AddSessionToDatabase(QJsonObject data);
    //接收服务器返回的查找数据信号
    void SearchInfoReceived(QJsonObject data);
public slots:


private:

    ClientSir(QString IPAddress, int port, QObject *parent = nullptr);
    QTcpSocket *socket;
    QString ipAddress_;
    int port_;
    Login * login = nullptr;
    UserRegister * userregister = nullptr;
    MainWindow * mainw = nullptr;
    ClientDataCenter & datacenter = ClientDataCenter::Singleton();
};

#endif // CLIENTSIR_H
