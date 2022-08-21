#include "messagemodel.h"
#include <QDebug>
//根据传入的信息打包到自身的保护属性
MessageModel::MessageModel(OnlineUserModel &senderUser, OnlineSession &sessionDest, QString msgText, QJsonObject msgProfile, QObject *parent): QObject(parent){
    senderUsername = senderUser.getUsername();
    sessionID = sessionDest.getSessionID();
    text = msgText;
    profile = msgProfile;
}
//根据传入的信息打包到自身的保护属性，拷贝构造函数
MessageModel::MessageModel(MessageModel *old){
    if (old == nullptr){
        return ;
    }
    senderUsername = old->senderUsername;
    text = old->text;
    profile = old->profile;
    sessionID = old->sessionID;
}
//用户在线时，要发送的信息构造函数
OnlineMessage::OnlineMessage(QJsonObject json, MessageModel *parent):MessageModel(parent){
    messageID = json["MessageID"].toInt();
    sessionID = json["SessionID"].toInt();
    senderUsername = json["SenderName"].toString();
    QJsonObject body = json["Body"].toObject();
    text = body["Text"].toString();
    profile = body["Profile"].toObject();
}

//抽取自身属性打包成json
QJsonObject MessageModel::generateJsonOutput() const{
    QJsonObject json = {{ "MsgType", "SessionMessage"}};
    json["MessageID"] = -1;
    json["SessionID"] = sessionID;
    json["SenderName"] = senderUsername;
    QJsonObject body = {{ "Text", text }};
    body["Profile"] = QJsonObject{{ "hasMentionInfo", false }};
    json["Body"] = body;
    return json;
}
//抽取自身属性打包成json
OnlineMessage::OnlineMessage(int MessageID, int SessionID, QString sender, QString messageText, QJsonObject Profile, MessageModel * parent)
{
    messageID = MessageID;
    sessionID = SessionID;
    senderUsername = sender;
    text = messageText;
    profile = Profile;
    this->setParent(parent);
}
