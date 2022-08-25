#include <QObject>
#include <QList>
#include <QString>
#include <QJsonArray>
#include "usermodel.h"

#ifndef ABSTRACTSESSION_H
#define ABSTRACTSESSION_H

using UserTypeName = QString;
using UserContainer = QList<UserTypeName>;

class AbstractSession : public QObject
{
    Q_OBJECT
public:
    explicit AbstractSession();

    enum class SessionType { FRIEND, GROUP };

    virtual SessionType getSessionType() const {
        return members.size() == 2 ? SessionType::FRIEND : SessionType::GROUP;
    }

    int getMemberCount() const { return members.size(); }

protected:
    UserContainer members;

signals:

};

#endif // ABSTRACTSESSION_H
