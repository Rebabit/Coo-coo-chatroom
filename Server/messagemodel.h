#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QObject>
#include "usermodel.h"
#include "Session/onlinesession.h"

class MessageModel : public QObject
{
    Q_OBJECT
public:
    explicit MessageModel(OnlineUserModel& senderUser, OnlineSession& sessionDest,
                          QString msgText, QJsonObject msgProfile, QObject *parent = nullptr);
    MessageModel(MessageModel * old = nullptr);
    int getSessionID() const;
    const OnlineSession & getSession() const;
    const QString& getSenderUsername() const;
    const OnlineUserModel& getSender() const;
    virtual int getMessageID() const { throw "Offline Message do not have messageID"; }

    const QString& getMessageText() const;
    const QJsonObject& getMessageProfile() const;

    enum class Type { Offline, Online };
    virtual Type getType() const { return Type::Offline; }



signals:

protected:
    QString senderUsername;
    int sessionID;
    QString text;
    QJsonObject profile;

};

class OnlineMessage : public MessageModel
{
    Q_OBJECT
public:
    explicit OnlineMessage(QJsonObject json, MessageModel * parent = nullptr);
    explicit OnlineMessage(int MessageID, int SessionID, QString sender, QString text, QJsonObject Profile, MessageModel * parent = nullptr);
    int getMessageID() const override { return messageID; }
    QJsonObject generateJsonOutput() const;
private:
    int messageID;
};

#endif // MESSAGEMODEL_H
