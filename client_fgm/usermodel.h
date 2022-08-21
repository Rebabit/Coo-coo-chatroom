#ifndef USERMODEL_H
#define USERMODEL_H

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>


//基础用户模型，包含最基本的用户名、昵称、profile的信息
class UserModel : virtual public QObject
{
    Q_OBJECT
public:
    UserModel(QObject * parent = nullptr);
    enum class Type {Offline, Online, None};   //在线状态
    virtual Type getType() const {return Type::None;}
    const QString& getUsername() const { return username; } //获取用户名
    const QJsonObject& getProfile() const { return profile; } //获取profile

signals:


protected:
    QString username;   //用户名
    QString nickname;   //昵称
    QJsonObject profile;  //profile

public slots:
};


//线下用户模型
class OfflineUserModel : virtual public UserModel, virtual public QObject{
    Q_OBJECT
public:
    OfflineUserModel(QString username_, QObject * parent = nullptr);
    Type getType() const {return Type::Offline;}
    void setNickname(QString nickname_) {nickname = nickname_;}
    void setSigniture(QString sig) {profile["signiture"] = sig;}
    QJsonObject generateUserModelJson() const;
signals:
};

//线上用户模型
class OnlineUserModel : virtual public UserModel, virtual public QObject{
    Q_OBJECT
public:
    OnlineUserModel(QJsonObject &json, QObject *parent = nullptr);
    OnlineUserModel(QString usrname, QString nicname, QJsonObject json, QObject * parent = nullptr);
    Type getType() const {return Type::Online;}
    const QString & getNickname() const {return nickname ;}
    const QString & getSigniture() const {return profile["signiture"].toString();}

    QJsonObject generateUserModelJson() const;
signals:

private:

};


class LocalUser : virtual public OnlineUserModel{
    Q_OBJECT
public:

signals:

private:
};


#endif // USERMODEL_H
