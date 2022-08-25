#ifndef OFFLINESESSION_H
#define OFFLINESESSION_H

#include <QObject>
#include "Session/abstractsession.h"

class OfflineSession: public AbstractSession
{
    Q_OBJECT

public:
     OfflineSession(UserContainer Users);
     OfflineSession(QList<UserModel*> Users);
};

#endif // OFFLINESESSION_H
