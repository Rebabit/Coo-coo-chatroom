#include "offlinesession.h"

OfflineSession::OfflineSession(UserContainer Users, AbstractSession * parent) : QObject(parent)
{
    if (Users.size() < 2) throw "ValueError";
    members = Users;
}

OfflineSession::OfflineSession(QList<UserModel*> Users, AbstractSession * parent) : QObject(parent)
{
    if (Users.size() < 2) throw "ValueError";
    for (int i = 0; i != Users.size(); i++) {
        members.append(Users.at(i)->getUsername());
    }
}
