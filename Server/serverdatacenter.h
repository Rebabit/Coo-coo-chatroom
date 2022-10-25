#ifndef SERVERDATACENTER_H
#define SERVERDATACENTER_H

#include <QObject>
#include <QSet>
#include <QMap>
#include <QPair>

#include "Session/abstractsession.h"
#include "Session/onlinesession.h"
#include "Session/offlinesession.h"

#include "usermodel.h"
#include "messagemodel.h"

class ServerDataCenter : public QObject
{
    Q_OBJECT
public:
    static ServerDataCenter& Singleton(QObject * parent = nullptr) {
        static ServerDataCenter * singleton = new ServerDataCenter(parent);
        return * singleton;
    }

    // 将用户模型插入到datacenter
    void registerUser(OnlineUserModel * newuser);
    void registerSession(OnlineSession * session);
    void registerMessage(OnlineMessage * msg);
    OnlineSession& getSession(int SessionId);
    OnlineUserModel & getUser(QString username);
    OnlineMessage & getMessage(int SessionId, int MessageId);
    bool isRegistered(QObject* obj) { return registeredObjects.contains(obj); }
    bool hasUser(QString username);
    bool hasSession(int sessionId);
    bool hasMessage(int sessionId, int messageId);

public slots:
    void clean();

private:
    explicit ServerDataCenter(QObject *parent = nullptr);
    QMap<QString, OnlineUserModel *> users;
    QList<QObject *> registeredObjects;
    QMap<int, OnlineSession *> sessions;
    QMap<QPair<int, int>, OnlineMessage *> messages;
    OnlineSession* _getSession(int SessionId);
    OnlineUserModel*  _getUser(QString username);
    OnlineMessage* _getMessage(int SessionId, int MessageId);

};


#endif // SERVERDATACENTER_H
