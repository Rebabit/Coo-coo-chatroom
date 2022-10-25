#include "sever.h"
#include"handlesignal.h"
#include<QDebug>
#include<QTimer>
#include<QMessageBox>
#include"operations.h"
#include<QJsonDocument>
#include"databaseoperation.h"


Sever::Sever(QObject *parent) : QTcpServer (parent)
{

}

void Sever::incomingConnection(qintptr handle)
{

    qDebug()<<"产生了新连接！！！";
    //设置新加入的socket并加入clientMap
    QTcpSocket *socket = new QTcpSocket(this);
    socket->setSocketDescriptor (handle);

    clientMap.insert (handle,socket);

    handleSignal *newHandle = new handleSignal(handle);

    connect(newHandle, &handleSignal::sendSignal, this,&Sever::receiveMessage);

    //以后的每次数据读入，将会触发handle信号，随后连锁反应会接受上面的信号
    connect (socket, &QTcpSocket::readyRead, [=](){
        newHandle->aaa(handle);
    });

    connect(socket,&QTcpSocket::disconnected,[=](){
        emit offLine (handle);
    });

    emit sendChannel (handle);

    emit linkMsg (socket->peerAddress ().toString ()+"连接成功！");     //2表示在登录框中显示

}

void Sever::setIP(QString str)
{

     if(listen(QHostAddress(str) ,8888))
     {
        DatabaseOperation& db = DatabaseOperation::Singleton();
        db.startDatabaseConnection ("Server.db");
        qDebug()<<"服务器成功开启！";
     }
}


void Sever::receiveMessage(qintptr handle)
{
    qDebug()<<"in receive message!";

    //根据不同handle信号找到应该接受信息的socket
    QMap<int, QTcpSocket*>::iterator it = clientMap.find (handle);
    QByteArray receiveMes = it.value ()->readAll ();
    QJsonDocument receJson = QJsonDocument::fromJson (receiveMes);
    QJsonObject recejson = receJson.object ();
    qDebug() << "Received Message from " << handle << ": " << recejson;
    if(!recejson.contains ("MsgType"))
    {
        qDebug()<<"receive message is not json!";

        return ;
    }
    QString method = recejson["MsgType"].toString().toLower();

    auto returnList =  QList<QJsonObject>();
    auto &op = Operations::Singleton ();

    if(method == "login")
    {
        //首次收到登陆信息
        messageSender = recejson["Username"].toString();
        QMap <QString, QList<int>>::iterator it1 = userToHandle.begin();
        int flag=0;
        while(it1!=userToHandle.end ())
        {
            if(it1.key ()==messageSender)//并非首次登陆
            {
                it1.value ().append (handle);
                emit linkMsg (messageSender+" "+QString::number (it1.value ().size ())+" "+"登陆了！");
                qDebug()<< it1.value ().size ();
                flag=1;
                break;
            }
            it1++;
        }
        if(flag==0)//首次登陆
        {
            QList<int> ll;
            ll.append (handle);
            userToHandle.insert (messageSender,ll);
            emit linkMsg (messageSender+" 1 "+"登陆了！");
        }
        returnList = op.loginResponse (recejson);
        emit sendMsg (returnList, handle);
    }
    else if (method == "regist") {
        returnList = op.registerResponse(recejson);
        emit sendMsg (returnList, handle);
    }
    else if (method == "info") {

        emit linkMsg(recejson["Message"].toString());

    }
    else if(method == "sessionmessage"){
        op.newMessageResponse (recejson);
        //同时触发信号，到widget
    }
    else if (method == "search") {
        returnList = op.searchResponse(recejson);
        emit sendMsg(returnList, handle);
    }
    else if (method == "addfriendrequest")//添加好友
    {
        QJsonObject result = op.addFriendResponse(recejson);
        emit sendMsg({result}, handle);
        qDebug() << result;
        if(result["Result"].toString().toLower() == "succeeded")
        {
            // 给被加好友的人的提示
            QJsonObject newResult;
            newResult["MsgType"] = "AddFriendNewRequest";
            newResult["FromUsername"] = recejson["FromUsername"];
            newResult["ToUsername"] = recejson["ToUsername"];
            QList<int> handleList = userToHandle.value(recejson["ToUsername"].toString());
            for(int i=0;i<handleList.size();i++)
            {
                emit sendMsg({newResult}, handleList[i]);
            }

        }
    }
    else if (method == "newgroup") {
        op.createGroupResponse(recejson);
    }
    else if (method == "info")//信息
    {
        emit linkMsg(recejson["Message"].toString());
    }
    else if (method == "addfriendresponse")// 被加好友的人的回应
    {
        QJsonObject result = op.addFriendResultResponse(recejson);
        qDebug() << "response发送给client" << result;
        QList<int> handleList = userToHandle.value(recejson["FromUsername"].toString());
        for(int i=0;i<handleList.size();i++)
        {
            qDebug() << "response发送给client" << result;
            emit sendMsg({result},handleList[i]);
        }
    }
    else if(method == "huantourequest"){
        qDebug() << "message huan tou recv";
        op.huantouResponse(recejson);
    }else if(method == "signaturechange"){
        op.changeSignatureResponse(recejson);
    }
    else if(method == "filesessionmsg"){
        op.newMessageResponse(recejson);
    }

    qDebug() << "Server ReturnList: " << returnList;

}
