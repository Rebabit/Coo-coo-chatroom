#ifndef OFFLINESESSION_H
#define OFFLINESESSION_H

#include <QObject>
#include "Session/abstractsession.h"

class OfflineSession: public AbstractSession
{
    Q_OBJECT

public:
     OfflineSession(UserContainer Users, AbstractSession * parent = nullptr);
     OfflineSession(QList<UserModel*> Users, AbstractSession * parent = nullptr);
};

#endif // OFFLINESESSION_H
