#ifndef ONLINESESSION_H
#define ONLINESESSION_H

#include <QObject>
#include "Session/abstractsession.h"

class OnlineSession: public AbstractSession
{
    Q_OBJECT

public:
     OnlineSession(QJsonObject &json);
     OnlineSession(int sessionID, QString sessionName, QJsonObject profile, UserContainer Users) {
         id = sessionID, SessionName = sessionName, Profile = profile, members = Users;
         latest = profile["LatestMessageID"].toInt();
     }

     int getSessionID() const { return id; }
     const QString& getSessionName() const;
     int & getLatestMessageID() { return latest; }
     const QJsonObject& getProfile() const { return Profile; }
     QJsonObject generateJsonFromData() const;
     void addMember(OnlineUserModel * user) { members.append(user->getUsername()); }

private:
     int id, latest;
     QString SessionName;
     QJsonObject Profile;

     void loadUsersFromJson(const QJsonObject& json);
     void loadDataFromJson(const QJsonObject& json);
};
#endif // ONLINESESSION_H
