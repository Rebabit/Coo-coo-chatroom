#include "messagemodel.h"
#include <QDebug>

MessageModel::MessageModel(OnlineUserModel& sender, OnlineSession& session, QString msgText, QJsonObject msgProfile, QObject *parent) : QObject(parent)
{
    senderUserName = sender.getUsername();
    sessionID = session.getSessionID();
    text = msgText;
    profile = msgProfile;
}

MessageModel::MessageModel(MessageModel * old)
{
    if(old == nullptr)
    {
        return;
    }
    senderUserName = old->senderUserName;
    sessionID = old->sessionID;
    text = old->text;
    profile = old->profile;
}

int MessageModel::getSessionID() const
{
    return sessionID;
}

const QString& MessageModel::getSenderUserName() const
{
    return senderUserName;
}

const QString& MessageModel::getMessageText() const
{
    return text;
}
const QJsonObject& MessageModel::getMessageProfile() const
{
    return profile;
}
int MessageModel::getMessageID() const
{
    throw "Offline message does not has messageID";
}

MessageModel::Type MessageModel::getType() const
{
    return Type::Offline;
}

OnlineMessage::OnlineMessage(QJsonObject json, MessageModel * parent) : MessageModel(parent)
{
    messageID = json["MessageID"].toInt();
    sessionID = json["SessionID"].toInt();
    senderUserName = json["SenderName"].toString();
    QJsonObject body = json["Body"].toObject();
    text = body["Text"].toString();
    profile = body["Profile"].toObject();
}

OnlineMessage::OnlineMessage(int MessageID, int SessionID, QString sender, QString msgText, QJsonObject Profile, MessageModel * parent)
{
    messageID = MessageID;
    sessionID = SessionID;
    senderUserName = sender;
    text = msgText;
    profile = Profile;
    this->setParent(parent);
}

int OnlineMessage::getMessageID() const
{
    return messageID;
}
QJsonObject OnlineMessage::generateJsonOutput() const
{
    QJsonObject json = {{ "MsgType", "SessionMessage" }};
    json["SessionID"] = sessionID;
    json["MessageID"] = messageID;
    json["SenderName"] = senderUserName;
    json["Body"] = QJsonObject({{"Text", text}, {"Profile", QJsonObject()}});
    return json;
}
