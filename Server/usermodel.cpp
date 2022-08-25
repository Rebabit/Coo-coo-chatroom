#include "usermodel.h"


UserModel::UserModel()
{
    profile["signiture"] = "Nop";
}

OfflineUserModel::OfflineUserModel(QString Username)
{
    username = Username;
}

OnlineUserModel::OnlineUserModel(QJsonObject &json)
{
    loadBasicInfoFromJson(json);
}

void OnlineUserModel::loadBasicInfoFromJson(QJsonObject &json) {
    username = json["Username"].toString();
    nickname = json["Nickname"].toString();
    profile = json["Profile"].toObject();
}


OnlineUserModel::OnlineUserModel(QString usrname, QString nick, QJsonObject json)
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
    json["MsgType"] = "UserData";
    json["Username"] = username;
    json["Nickname"] = nickname;
    json["Profile"] = profile;
    return json;
}
