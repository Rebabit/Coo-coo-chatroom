#ifndef USERMODEL_H
#define USERMODEL_H

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

class UserModel : virtual public QObject
{
    Q_OBJECT
public:
    UserModel();
    enum class Type { Offline, Online, None};
    virtual Type getType() const { return Type::None; }
    const QString& getUsername() const { return username; }
    const QJsonObject& getProfile() const { return profile; }

    QString username;
    QString nickname;
    QJsonObject profile;
signals:

protected:

};

class OfflineUserModel : public UserModel
{
    Q_OBJECT
public:
    OfflineUserModel(QString Username);
    Type getType() const { return Type::Offline; }
    void setNickname(QString nname) { nickname = nname; }
    void setSigniture(QString sig) { profile["signiture"] = sig; }
    QJsonObject generateUserModelJson() const;



signals:

};


class OnlineUserModel : public UserModel
{
    Q_OBJECT
public:
    OnlineUserModel(){};
    OnlineUserModel(QJsonObject &json);
    OnlineUserModel(QString usrname, QString nick, QJsonObject json);
    Type getType() const { return Type::Online; }
    const QString& getNickname() const { return nickname; }
    const QString getSigniture() const { return profile["signiture"].toString(); }
    QString getTouxiang()  {return profile["touxiangURL"].toString(); }
    void setTouxiang(QString _touxiangurl) {
        profile["touxiangURL"] = _touxiangurl;
    }
    QJsonObject generateUserModelJson() const;
signals:

private:
    void loadBasicInfoFromJson(QJsonObject &json);
};

class LocalUser : virtual public OnlineUserModel
{
    Q_OBJECT
public:


signals:

private:
};

#endif // USERMODEL_H
