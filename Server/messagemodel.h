#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QObject>
#include "usermodel.h"
#include "Session/onlinesession.h"

class MessageModel : public QObject
{
    Q_OBJECT
public:
    explicit MessageModel(OnlineUserModel& sender, OnlineSession& session, QString msgText, QJsonObject msgProfile, QObject *parent = nullptr);
    MessageModel(MessageModel *old = nullptr);
    int getSessionID() const;
    const OnlineSession& getSession() const; // not implemented.
    const QString& getSenderUserName() const;
    const OnlineUserModel& getSender() const; // not implemented.
    const QString& getMessageText() const;
    const QJsonObject& getMessageProfile() const;
    virtual int getMessageID() const; // if Offline, don't have messageID

    enum class Type {Offline, Online};
    virtual Type getType() const;

protected:
    QString senderUserName;
    int sessionID;
    QString text;
    QJsonObject profile;
};

class OnlineMessage : virtual public MessageModel
{
    Q_OBJECT
public:
    explicit OnlineMessage(QJsonObject json, MessageModel * parent = nullptr);
    explicit OnlineMessage(int MessageID, int SessionID, QString sender, QString msgText, QJsonObject Profile, MessageModel * parent = nullptr);
    int getMessageID() const override;
    QJsonObject generateJsonOutput() const;
private:
    int messageID; //doesn't called.
};

#endif // MESSAGEMODEL_H
