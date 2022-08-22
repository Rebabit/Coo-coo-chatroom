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
    enum class Type{Online, Offline, Unknown};
    virtual Type getType() const; //for UserModel, return Unknown.
    const QString& getUsername() const;
    void setUsername(QString newName);
    const QString& getNickname() const;
    void setNickname(QString newName);
    const QJsonObject& getProfile() const;

protected:
    QString userName;
    QString nickName;
    QJsonObject profile;
};

class OfflineUserModel : virtual public UserModel
{
    Q_OBJECT
public:
    OfflineUserModel(QString UserName);
    Type getType() const; // Offline.
    void setSigniture(QString sig);
    QJsonObject generateUserModelJson() const;

};

class OnlineUserModel : virtual public UserModel
{
    Q_OBJECT
public:
    OnlineUserModel(QJsonObject &json);
    OnlineUserModel(QString username, QString nickname, QJsonObject json);
    Type getType() const; // Online.
    const QString getSigniture() const;
    QJsonObject generateUserModelJson() const;

private:
    void loadBasicInfoFromJson(QJsonObject& json);
};

#endif // USERMODEL_H
