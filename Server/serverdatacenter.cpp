#include "serverdatacenter.h"
#include <QDebug>

void ServerDataCenter::registerUser(OnlineUserModel * newuser) {
    if (users.contains(newuser->getUsername())) {
        return;
    }
    users[newuser->getUsername()] = newuser;
    newuser->setParent(this);
    registeredObjects.append(newuser);

}


void ServerDataCenter::registerSession(OnlineSession *session) {
    if (sessions.contains(session->getSessionID())) {
        return;
    }
    sessions[session->getSessionID()] = session;
    session->setParent(this);
    registeredObjects.append(session);
}

void ServerDataCenter::registerMessage(OnlineMessage *msg) {
    if (messages.contains({msg->getSessionID(), msg->getMessageID()})) {
        return;
    }
    messages[{msg->getSessionID(), msg->getMessageID()}] = msg;
    msg->setParent(this);
    registeredObjects.append(msg);
}

bool ServerDataCenter::hasUser(QString username) {
    return _getUser(username) != nullptr;
}
bool ServerDataCenter::hasSession(int sessionId) {
    return _getSession(sessionId) != nullptr;
}
bool ServerDataCenter::hasMessage(int sessionId, int messageId) {
    return _getMessage(sessionId, messageId) != nullptr;
}

void ServerDataCenter::clean() {
    users.clear();
    sessions.clear();
    messages.clear();
    registeredObjects.clear();
}
