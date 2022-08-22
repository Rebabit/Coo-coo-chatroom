#include "usermodel.h"

// UserModel
UserModel::UserModel()
{
    profile["signiture"] = "Nop";
}

UserModel::Type UserModel::getType() const
{
    return Type::Unknown;
}

const QString& UserModel::getUsername() const
{
    return userName;
}
void UserModel::setUsername(QString newName)
{
    userName = newName;
}

const QString& UserModel::getNickname() const
{
    return nickName;
}
void UserModel::setNickname(QString newName)
{
    nickName = newName;
}

const QJsonObject& UserModel::getProfile() const
{
    return profile;
}

//OfflineUserModel
OfflineUserModel::OfflineUserModel(QString UserName)
{
    userName = UserName;
}
UserModel::Type OfflineUserModel::getType() const
{
    return Type::Offline;
}
void OfflineUserModel::setSigniture(QString sig)
{
    profile["signiture"] = sig;
}
QJsonObject OfflineUserModel::generateUserModelJson() const
{
    QJsonObject json;
    json["MsgType"] = "UserData";
    json["Username"] = userName;
    json["Nickname"] = nickName;
    json["Profile"] = profile;
    return json;
}

//onlineUserModel
OnlineUserModel::OnlineUserModel(QJsonObject &json)
{
    loadBasicInfoFromJson(json);
}

OnlineUserModel::OnlineUserModel(QString username, QString nickname, QJsonObject pro)
{
    userName = username;
    nickName = nickname;
    profile = pro;
}

UserModel::Type OnlineUserModel::getType() const
{
    return Type::Online;
}
const QString OnlineUserModel::getSigniture() const
{
    return profile["signiture"].toString();
}
QJsonObject OnlineUserModel::generateUserModelJson() const
{
    QJsonObject json;
    json["MsgType"] = "UserData";
    json["Username"] = userName;
    json["Nickname"] = nickName;
    json["Profile"] = profile;
    return json;
}
void OnlineUserModel::loadBasicInfoFromJson(QJsonObject& json)
{
    userName = json["Username"].toString();
    nickName = json["Nickname"].toString();
    profile = json["Profile"].toObject();
}
