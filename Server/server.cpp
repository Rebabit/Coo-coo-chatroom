#include"server.h"
#include"handlesignal.h"
#include"operations.h"
#include"databaseoperation.h"

void Server::incomingConnection(qintptr handle)
{
    qDebug() << "new connection!";
    //建立新的套接字连接，初始化QTcpSocket对象
    QTcpSocket *socket = new QTcpSocket(this);
    //采用handle作为新连接的套接字描述符
    socket->setSocketDescriptor(handle);
    //将handle和对应的socket存入clientMap
    clientMap.insert(handle,socket);

    //将sendSignal信号与接收信息操作连接（根据handle数值区分）
    handleSignal *newHandle = new handleSignal(handle);
    connect(newHandle, &handleSignal::sendSignal, this, &Server::receiveMessage);
    //接受到新数据到达的信号后，发出sendSignal的信号
    connect(socket, &QTcpSocket::readyRead,[=](){
        newHandle->emitSignal(handle);
    });
    //接受到客户端断开的信号，发出offLine的信号
    connect(socket,&QTcpSocket::disconnected,[=](){
        emit offLine (handle);
    });
    //将每个服务器对应的handle显示于combobox待选
    emit sendChannel (handle);
    //将连接成功的信号显示于文本框
    emit linkMsg (socket->peerAddress ().toString ()+"连接成功！");     //2表示在登录框中显示
}

void Server::setIP(QString str)
{
    //如果监听到IP和对应端口
    if (listen(QHostAddress(str),8888))
    {
        //开启数据库
        DatabaseOperation& db = DatabaseOperation::Singleton();
        db.startDatabaseConnection("Server.db");
        qDebug()<<"服务器成功开启";
    }
}

//处理新信息
void Server::receiveMessage(qintptr handle)
{
    qDebug() << "in receive message!";
    //根据handle标识符找到接受信息的socket
    QMap<int, QTcpSocket*>::iterator it = clientMap.find(handle);
    //收到的信息
    QByteArray receiveMes = it.value()->readAll();
    QJsonObject recejson = QJsonDocument::fromJson (receiveMes).object ();
    qDebug() << "Received Message from " << handle << ": " << recejson;
    if(!recejson.contains ("MsgType"))
    {
        qDebug()<<"receive message is not json!";
        return ;
    }
    //信息的类别
    QString method = recejson["MsgType"].toString().toLower();
    auto returnList =  QList<QJsonObject>();
    auto &op = Operations::Singleton ();
    if (method == "login")//支持登录多个账号
    {
        messageSender = recejson["Username"].toString();
        QMap<QString,QList<int>>::iterator it1 = userToHandle.begin();
        int flag = 0;
        //根据用户名找到对应的handle的list。如果找到，则并非首次登录
        while (it1!=userToHandle.end())
        {
            if (it1.key() == messageSender)
            {
                it1.value().append(handle);
                emit linkMsg (messageSender+" "+QString::number (it1.value ().size ())+" "+"尝试登陆了！");
                qDebug()<< it1.value ().size ();
                flag=1;
                break;
            }
            it1 ++;
        }
        if (flag == 0)
        {
            QList<int> newList;
            newList.append(handle);
            userToHandle.insert(messageSender,newList);
            emit linkMsg (messageSender+" 1 "+"尝试登陆了！");
        }
        returnList = op.loginResponse(recejson);
        emit sendMsg(returnList,handle);
    }
    else if (method == "regist")//注册
    {
        returnList = op.registerResponse(recejson);
        emit sendMsg (returnList, handle);
    }
    else if (method == "search")//查找好友
    {
        returnList = op.searchResponse(recejson);
        emit sendMsg(returnList, handle);
    }
    else if (method == "addfriendrequest")//添加好友
    {
        op.addFriendResponse(recejson);
    }
    else if (method == "newgroup")//创建群聊
    {
        op.createGroupResponse(recejson);
    }
    else if (method == "info")//信息
    {
        emit linkMsg(recejson["Message"].toString());
    }
    qDebug() << "Server ReturnList: " << returnList;
}
