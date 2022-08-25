#include "clientdatacenter.h"
#include "kuang.h"
#include <QDebug>


void ClientDataCenter::registerUser(OnlineUserModel * newuser) {
    if (users.contains(newuser->getUsername())) {
        return;
    }
    users[newuser->getUsername()] = newuser;
    newuser->setParent(this);
    registeredObjects.append(newuser);

}

void ClientDataCenter::GetSessionMessage(){
    if(Kuang::KuangChosenNow){
        int SessionID = Kuang::KuangChosenNow->SessionID;
        if(hasSession(SessionID)){
            OnlineSession &temp = getSession(Kuang::KuangChosenNow->SessionID);
            for(int i = 1;i <= temp.getLatestMessageID();i++){
                if(hasMessage(SessionID,i)){
                    QJsonObject tem = getMessage(SessionID,i).generateJsonOutput();
                    emit addsessionmessage(tem);
                }
            }
        }
    }
}

void ClientDataCenter::registerSession(OnlineSession *session) {
    if (sessions.contains(session->getSessionID())) {
        return;
    }
    sessions[session->getSessionID()] = session;
    session->setParent(this);
    registeredObjects.append(session);
}

void ClientDataCenter::registerMessage(OnlineMessage *msg) {
    if (messages.contains({msg->getSessionID(), msg->getMessageID()})) {
        return;
    }
    messages[{msg->getSessionID(), msg->getMessageID()}] = msg;
    msg->setParent(this);
    registeredObjects.append(msg);
    qDebug() << "###  ClientDataCenterDown";
}

void ClientDataCenter::RegisterSession(QJsonObject data){

    if(!hasSession(data["SessionID"].toInt())){

        OnlineSession *temp = new OnlineSession(data);
        registerSession(temp);
//         qDebug() << data["SessionType"].toString() << "\n";
        if(data["SessionType"].toString()=="FRIEND"){
            emit FriendSessionDataReceived(data);
        }
        if(data["SessionType"].toString()=="GROUP"){
            emit GroupSessionDataReceived(data);
        }
    }
}

void ClientDataCenter::RegisterMessage(QJsonObject data){
    if(!hasMessage(data["SessionID"].toInt(),data["MessageID"].toInt())){
        OnlineMessage *temp = new OnlineMessage(data);
        registerMessage(temp);
        getSession(data["SessionID"].toInt()).getLatestMessageID() = data["MessageID"].toInt();
        emit sessionorderchange(data);
    }
}

bool ClientDataCenter::hasUser(QString username) {
    return _getUser(username) != nullptr;
}

bool ClientDataCenter::hasSession(int sessionId) {
    return _getSession(sessionId) != nullptr;
}
bool ClientDataCenter::hasMessage(int sessionId, int messageId) {
    return _getMessage(sessionId, messageId) != nullptr;
}

void ClientDataCenter::clean() {
    users.clear();
    sessions.clear();
    messages.clear();
    registeredObjects.clear();
}
