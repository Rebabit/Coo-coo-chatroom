#include "offlinesession.h"

OfflineSession::OfflineSession(UserContainer Users)
{
    if (Users.size() < 2) throw "ValueError";
    members = Users;
}

OfflineSession::OfflineSession(QList<UserModel*> Users)
{
    if (Users.size() < 2) throw "ValueError";
    for (int i = 0; i != Users.size(); i++) {
        members.append(Users.at(i)->getUsername());
    }
}
