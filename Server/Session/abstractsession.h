#include <QObject>
#include <QJsonArray>
#include "usermodel.h"

#ifndef ABSTRACTSESSION_H
#define ABSTRACTSESSION_H

using UserTypeName = QString;
using UserContainer = QList<UserTypeName>;

class AbstractSession : virtual public QObject
{
    Q_OBJECT
public:
    explicit AbstractSession(QObject *parent = nullptr);

    enum class SessionType { FRIEND, GROUP };

    virtual SessionType getSessionType() const {
        return members.size() == 2 ? SessionType::FRIEND : SessionType::GROUP;
    }

    int getMemberCount() const { return members.size(); }
    const UserContainer & getMembers() { return members; }

protected:
    UserContainer members;

signals:

};

#endif // ABSTRACTSESSION_H
