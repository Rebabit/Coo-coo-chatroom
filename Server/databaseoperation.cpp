#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QFileInfo>
#include <QTextCodec>

#include "databaseoperation.h"

OnlineUserModel* ClientDataCenter::_getUser(QString username) {
    if (users.contains(username))
        return users[username];
    return nullptr;
}

OnlineSession* ClientDataCenter::_getSession(int SessionId) {
    if (sessions.contains(SessionId))
        return sessions[SessionId];
    return nullptr;
}

OnlineMessage* ClientDataCenter::_getMessage(int SessionId, int MessageId) {
    if (messages.contains(QPair<int, int>({SessionId, MessageId})))
        return messages[QPair<int, int>({SessionId, MessageId})];
    return nullptr;
}


OnlineUserModel & ClientDataCenter::getUser(QString username) {
    if (_getUser(username) == nullptr) throw "Not exist";
    return *users[username];
}

OnlineSession & ClientDataCenter::getSession(int SessionId) {
    if (_getSession(SessionId) == nullptr) throw "Not exist";
    return *sessions[SessionId];
}

OnlineMessage & ClientDataCenter::getMessage(int SessionId, int MessageId) {
    if (_getMessage(SessionId, MessageId) == nullptr) throw "Not exist";
    return *messages[{SessionId, MessageId}];
}
