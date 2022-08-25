#ifndef DATABASEOPERATION_H
#define DATABASEOPERATION_H

#include <QObject>
#include <QSqlDatabase>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QString>

#include "clientdatacenter.h"

QString json2str(QJsonObject json);

QJsonObject str2json(QString str);

class DatabaseOperation : public QObject
{
    Q_OBJECT
public:
    static DatabaseOperation & Singleton(QObject * parent = nullptr) {
        static DatabaseOperation * singleton = new DatabaseOperation(parent);
        return *singleton;
    }

    void startDatabaseConnection(QString dbfilename);
    bool isDBExist(QString str) const;
    void closeDB();
    int getTableCount(QString tableName) const;
    QList<OnlineUserModel*> findAllUsers();
    QList<OnlineSession*> findAllSessions();
    QList<OnlineMessage*> findAllMessages();

    bool insertUser(QString username, QString nickname, QString password, QJsonObject profile);
    bool insertUser(const OnlineUserModel &user, const QString &password);
    // 返回SessionID
    int insertSessionBasicInfo(QString sessionType,  QJsonObject profile);
    bool isRunning() const { return status == Status::Running; }
    bool insertMember(int sessionID, const OnlineUserModel& user);
    bool insertMember(int sessionID, QString user);
    int insertNewMessage(int SessionId, QString senderUsername, QString MessageText, QJsonObject profile);

    QList<QString> queryMembersBySession(int sessionID);
    QList<int> querySessionsByMember(QString username);
    QList<int> getMessageListBySessionID(int SessionId) const;

    OnlineMessage * findMessage(int sessionId, int MessageId);
    OnlineSession * findSession(int sessionID);
    OnlineUserModel * findUser(QString username);

    bool attemptLogIn(QString username, QString password) const;

signals:
    void signal_DBstop();
    void signal_DB_ready();

private:
    explicit DatabaseOperation(QObject *parent = nullptr);
    enum class Status { Stop, Running } status;
    void executeSqlStatement(QString str);
    void createTables();

    QSqlDatabase database;
};


#endif // DATABASEOPERATION_H
