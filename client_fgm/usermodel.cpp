#include "usermodel.h"

UserModel::UserModel(QObject *parent) : QObject(parent)
{
}

OfflineUserModel::OfflineUserModel(QString username_, QObject *parent):QObject(parent){
    username = username_;
}

OnlineUserModel::OnlineUserModel(QJsonObject &json, QObject * parent) : QObject(parent){
    username = json["Username"].toString();
    nickname = json["Nickname"].toString();
    profile = json["Profile"].toObject();
}


OnlineUserModel::OnlineUserModel(QString usrname, QString nicname, QJsonObject json, QObject *parent) : UserModel(parent){
    username = usrname;
    nickname = nicname;
    profile = json;
}

QJsonObject OnlineUserModel::generateUserModelJson() const{
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
