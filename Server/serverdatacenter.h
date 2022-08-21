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

class ServerDataCenter: public QObject
//为了使用信号槽，必须继承QObject
//凡是QObject类（不管是直接子类还是间接子类），都应该在第一行代码写上Q_OBJECT
{
    Q_OBJECT
public:
    static ServerDataCenter& Singleton(QObject * parent = nullptr){
        static ServerDataCenter * singleton = new ServerDataCenter(parent);
        return * singleton;
    }//单例类Singleton保证同一时刻最多存在该类的一个对象

    //用户模型插入datacenter
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
