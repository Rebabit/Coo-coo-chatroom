#include "onlinesession.h"
#include <QDebug>

OnlineSession::OnlineSession(QJsonObject &json)
{
    loadDataFromJson(json);
}

void OnlineSession::loadDataFromJson(const QJsonObject &json)
{
    id = json["SessionID"].toInt();
    loadUsersFromJson(json);
    Profile = json["Profile"].toObject();
    if ((getSessionType() == SessionType::GROUP && json["SessionType"] == "GROUP")
     || (getSessionType() == SessionType::FRIEND && json["SessionType"] == "FRIEND") )
    {
        if (getSessionType() == SessionType::GROUP) {
            SessionName = Profile["SessionName"].toString();
        }
        else {
            SessionName = "None";
        }
    }
    else {
        qDebug() << "load error" ;
    }
    latest = Profile["LatestMessageID"].toInt();
}

const QString& OnlineSession::getSessionName() const {
    if (getSessionType() == SessionType::FRIEND)
        throw "FriendSession does not have a session name";
    return SessionName;
}

void OnlineSession::loadUsersFromJson(const QJsonObject& json)
{
    QJsonArray userlist = json["Members"].toArray();

    for (int i = 0; i < userlist.size(); i++) {
        QString usrname = userlist.at(i).toObject()["Username"].toString();
        members.append(usrname);
    }
    qDebug() << "Load " << userlist.size() << " users from list";
}

QJsonObject OnlineSession::generateJsonFromData() const {
    QJsonObject ret;
    ret["MsgType"] = "SessionData";
    ret["SessionID"] = id;
    ret["SessionType"] = getSessionType() == SessionType::GROUP ? "GROUP" : "FRIEND";
    QJsonArray userlist;
    for (int i = 0; i < members.size(); i++) {
        QJsonObject tmp;
        tmp["Username"] = members.at(i);
        userlist.append(tmp);
    }
    ret["Members"] = userlist;
    Profile["LatestMessageID"] = latest;
    ret["Profile"] = Profile;
    return ret;
}
