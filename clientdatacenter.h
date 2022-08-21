#ifndef SERVERDATACENTER_H
#define SERVERDATACENTER_H

#include <QObject>
#include <QSet>
#include <QMap>
#include <QPair>

#include "Session/abstractsession.h"
#include "Session/onlinesession.h"
#include "Session/offlinesession.h"

#include "usermodel.h"C
#include "messagemodel.h"

class ClientDataCenter
{
    Q_OBJECT
public:
    static ClientDataCenter& Singleton(QObject * parent = nullptr)
    {
        static ClientDataCenter* singleton = new ClientDataCenter(parent);
        return *singleton;
    }

    void registerUser(OnlineUserModel * newuser);
    void registerSession(OnlineSession * session);
    void registerMessage(OnlineMessage * msg);
    void RegisterSession(QJsonObject data);
    void RegisterMessage(QJsonObject data);
    //以下三个函数在databaseoperation中实现
    OnlineSession& getSession(int SessionId);
    OnlineUserModel & getUser(QString username);
    OnlineMessage & getMessage(int SessionId, int MessageId);
    bool hasUser(QString username);
    bool hasSession(int sessionId);
    bool hasMessage(int sessionId, int messageId);
    void GetSessionMessage();

public slots://自定义槽函数
    void clean();


private:
    explicit ClientDataCenter(QObject *parent = nullptr);
    QMap<QString, OnlineUserModel *> users;
    QList<QObject *> registeredObjects;
    QMap<int, OnlineSession *> sessions;
    QMap<QPair<int, int>, OnlineMessage *> messages;
    OnlineSession* _getSession(int SessionId);
    OnlineUserModel*  _getUser(QString username);
    OnlineMessage* _getMessage(int SessionId, int MessageId);
};

#endif // CLIENTDATACENTER_H
