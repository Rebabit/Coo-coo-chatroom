#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <QObject>
#include <QHostAddress>
#include <QHostInfo>
#include <QDebug>
#include <QStringList>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>

class TCP_Server : public QTcpServer
{
    Q_OBJECT
public:
    // 获取单例
    static TCP_Server& Singleton(QTcpServer * parent = nullptr) {

        static TCP_Server * singleton = new TCP_Server(parent);

        return *singleton;
    }

    // 运行服务器，需要保证当前服务器处于停止状态
    void runServer(QString IP_Address, quint16 port);
    // 停止服务器，需要保证当前服务器处于运行状态
    void stopServer();

    // 获取服务器运行状态，true代表服务器正常运行
    bool isRunning() const { return state_isRunning; }
    // 获取本机IP地址列表，这个方法预计在后续阶段中分离
    const QStringList& getIPList() const { return ipAddressList; }

//    // 终端实例对象
//    QTcpSocket *socket;
    virtual void incomingConnection(qintptr mmm) override;

signals:
    void signal_IP_ListUpdate();	// 初始化ip地址列表完成
    void signal_ServerStart();		// 服务器开始运行
    void signal_Error(QString str);	// 服务器出现异常，当前并未接入qt的错误信息
    void signal_ServerStop();		// 服务器停止运行

    void sendMsg(QString, int);          //将tcp_server收到的信息作为信号发送给mianwindow

private slots:
    // 初始化获取ip地址列表完成
    void slotUpdateIPList(QHostInfo info);

    //接收信息
    void receiveMessage();
    //有新的连接接入


private:
    // 禁用公有的ctor
    explicit TCP_Server(QTcpServer *parent = nullptr);

    // 服务器状态
    bool state_isRunning = false;

    // 本机IP地址列表，受 signal_IP_ListUpdate() 更新
    QStringList ipAddressList;

    // 本机Host信息
    QHostInfo localhostInfo;

    // 服务器实例对象
    QTcpServer * serverInstance;
    QList< QTcpSocket*> clientList;
protected:



};

#endif // TCP_SERVER_H
