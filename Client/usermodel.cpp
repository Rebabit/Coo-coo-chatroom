#include "usermodel.h"


UserModel::UserModel(QObject *parent) : QObject(parent)
{
    profile["signiture"] = "Nop";
    profile["touxiangURL"] = "Nop";
}

OfflineUserModel::OfflineUserModel(QString Username, QObject *parent) :QObject (parent)
{
    username = Username;
}

OnlineUserModel::OnlineUserModel(QJsonObject &json, QObject *parent) : QObject (parent)
{
    loadBasicInfoFromJson(json);
}

void OnlineUserModel::loadBasicInfoFromJson(QJsonObject &json) {
    username = json["Username"].toString();
    nickname = json["Nickname"].toString();
    profile = json["Profile"].toObject();
}


OnlineUserModel::OnlineUserModel(QString usrname, QString nick, QJsonObject json, QObject * parent) :
  UserModel(parent)
{
    username = usrname;
    nickname = nick;
    profile = json;
}


QJsonObject OnlineUserModel::generateUserModelJson() const {
    QJsonObject json;
    json["MsgType"] = "UserData";
    json["Username"] = username;
    json["Nickname"] = nickname;
    json["Profile"] = profile;
    return json;
}

QJsonObject OfflineUserModel::generateUserModelJson() const {
    QJsonObject json;
    json["Username"] = username;
    json["Nickname"] = nickname;
    json["Profile"] = profile;
    return json;
}
