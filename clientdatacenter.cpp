#include "ClientDataCenter.h"
#include "kuang.h"
#include <QDebug>

void ClientDataCenter::registerUser(OnlineUserModel *newuser)
{
    if(users.contains(newuser->getUsername())){
        return;//已有该用户的注册
    }
//QMap<QString, OnlineUserModel *> users;
//username索引的已注册用户信息列表
    users[newuser->getUsername()] = newuser;
    newuser->setParent(this);
//QList<QObject *> registeredObjects;
//当前在线的用户信息列表，加入新注册的用户
    registeredObjects.append(newuser);
}

void ClientDataCenter::registerSession(OnlineSession *session)
//开启一段新的会话
{
    if (sessions.contains(session->getSessionID())) {
        return;//已有相同ID的会话，无需新建
    }
    sessions[session->getSessionID()] = session;
    session->setParent(this);
//已开启的会话列表
    registeredObjects.append(session);
}

void ClientDataCenter::RegisterSession(QJsonObject data)
{
    if(!hasSession(data["SessionID"].toInt()))
    {
        OnlineSession *temp = new OnlineSession(data);
        registerSession(temp);
        //新会话加入sessions列表和registeredObjects列表
        //依据会话type，emit不同信号
        if(data["SessionType"].toString()=="FRIEND"){
            emit FriendSessionDataReceived(data);
        }
        if(data["SessionType"].toString()=="GROUP"){
            emit GroupSessionDataReceived(data);
        }
    }
}

void ClientDataCenter::registerMessage(OnlineMessage * msg)
//固定会话中发送新消息
{
    if (messages.contains({msg->getSessionID(), msg->getMessageID()})) {
        return;
    }//每个message用msgID和sessionID共同标记
    messages[{msg->getSessionID(), msg->getMessageID()}] = msg;
    msg->setParent(this);
    registeredObjects.append(msg);
    qDebug() << "###  ClientDataCenterDown";
}

void ClientDataCenter::RegisterMessage(QJsonObject data){
    if(!hasMessage(data["SessionID"].toInt(),data["MessageID"].toInt())){
        OnlineMessage *temp = new OnlineMessage(data);
        registerMessage(temp);
        //更新当前会话的LatestMessageID
//int & getLatestMessageID() { return latest; }
        getSession(data["SessionID"].toInt()).getLatestMessageID() = data["MessageID"].toInt();
        emit sessionorderchange(data);
    }
}

bool ClientDataCenter::hasUser(QString Username)
{
//_getUser,_getSession,_getMessage函数在databaseopeartion中实现
    return _getUser(username) != nullptr;
}

bool ClientDataCenter::hasSession(int sessionId) {
    return _getSession(sessionId) != nullptr;
}
bool ClientDataCenter::hasMessage(int sessionId, int messageId) {
    return _getMessage(sessionId, messageId) != nullptr;
}

void ClientDataCenter::GetSessionMessage()
{//显示选中kuang的聊天信息
     if(Kuang::KuangChosenNow)
     {
        int SessionID = Kuang::KuangChosenNow->SessionID;
        if(hasSession(SessionID))
        {
            OnlineSession &temp = getSession(Kuang::KuangChosenNow->SessionID);
            for(int i=1;i<=temp.getLatestMessageID();i++)
            {//显示当前会话的所有历史聊天记录
                if(hasMessage(SessionID,i))
                {
                    QJsonObject tem = getMessage(SessionID,i).generateJsonOutput();
                    //逐条返回消息记录
                    emit addsessionmessage(tem);
                }
            }
        }
     }
}

void ClientDataCenter::clean() {
    users.clear();
    sessions.clear();
    messages.clear();
    registeredObjects.clear();
}

