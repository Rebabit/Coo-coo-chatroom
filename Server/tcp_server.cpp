#include "tcp_server.h"
#include<QByteArray>
#include<QString>
#include<QDebug>
TCP_Server::TCP_Server(QObject *parent) : QTcpServer (parent)
{
    qDebug() << "localhost ServerName = " << localhostInfo.localHostName();
    localhostInfo.setHostName(localhostInfo.localHostName());
    // 获取本机 ip 地址列表
    QHostInfo::lookupHost(localhostInfo.localHostName(),
                          this, SLOT(slotUpdateIPList(QHostInfo)));
    // 创建服务器实例对象
    serverInstance = new QTcpServer();
//    connect(serverInstance,&QTcpServer::newConnection,this,&TCP_Server::incomingConnection);
}


void TCP_Server::slotUpdateIPList(QHostInfo info)
{
    // 从 info 中获取 所有的 ip 地址
    QList<QHostAddress> localhostAddressList
            = info.addresses();

    if(localhostAddressList.count() > 0){
        for (int i = 0; i < localhostAddressList.count(); ++i) {
            //  添加到 ipAddressList 中
            if (localhostAddressList.at(i).protocol() == QAbstractSocket::IPv4Protocol)
                ipAddressList.append(localhostAddressList.at(i).toString());
        }
    }
    emit signal_IP_ListUpdate();
}


void TCP_Server::runServer(QString IP_Address, quint16 port)
{
    // 若当前服务器处于运行状态
    if (state_isRunning) return;
    // 开启服务器
    if (serverInstance->listen(QHostAddress(IP_Address), port)) {
        qDebug() << "Server start on " << IP_Address.toUtf8().data() << " : " << port;
        state_isRunning = true;
        emit signal_ServerStart();
    }
    else {
        // 启动异常
        emit signal_Error("Server not started");
    }

}

void TCP_Server::stopServer()
{
    if (!state_isRunning) {
        // 服务器不在运行状态时停止
        emit signal_Error("Calling stopserver when server not running");
        return;
    }
    serverInstance->close();
    emit signal_ServerStop();
}

void TCP_Server::incomingConnection(qintptr mmm)
{
    qDebug()<<"产生了新连接！！！";
    //重写产生新连接
    QTcpSocket *socket = new QTcpSocket(this);
    socket->setSocketDescriptor (mmm);


    clientList.append (socket);
    connect(socket, &QTcpSocket::readyRead, this,&TCP_Server::receiveMessage);

    emit sendMsg (socket->peerAddress ().toString (), 2);
}

void TCP_Server::receiveMessage()
{
    QTcpSocket *socket = new QTcpSocket;
    //接收到消息之后
    QByteArray arr = socket->readAll ();
    QString receiveMes = arr.data ();

    emit sendMsg (receiveMes, 1);
}
