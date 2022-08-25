#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QFileInfo>
#include <QTextCodec>

#include "databaseoperation.h"

QJsonObject str2json(const QString jsonString)
{
    QTextCodec *tc = QTextCodec::codecForName("UTF-8");
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8().data());
    if (jsonDocument.isNull()){
        qDebug() << "Error Parsing Str to Json";
    }
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}


QString json2str(QJsonObject json) {
    return QString(QJsonDocument(json).toJson(QJsonDocument::Compact));
}

DatabaseOperation::DatabaseOperation(QObject *parent) : QObject(parent)
{
    status = Status::Stop;
}

void DatabaseOperation::startDatabaseConnection(QString dbfilename) {
    if (status != Status::Stop) {
        qDebug() << "db server already running...";
        throw "Already running error";
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbfilename);    //如果本目录下没有该文件,则会在本目录下生成,否则连接该文件
    if (!db.open()) {
           qDebug() << db.lastError().text();
           throw "Database Error";
    }
    status = Status::Running;
    createTables();
    findAllUsers();
    findAllSessions();
    findAllMessages();
    emit signal_DB_ready();
}

void DatabaseOperation::executeSqlStatement(QString str) {
    QSqlQuery query (str);
    if (!query.isActive()) {
        qDebug() << query.lastError();
    }
}

void DatabaseOperation::createTables() {
    executeSqlStatement("CREATE TABLE User(Username TEXT PRIMARY KEY, Nickname TEXT NOT NULL, Password TEXT NOT NULL, Profile TEXT)");
    executeSqlStatement("CREATE TABLE Session(SessionID INT PRIMARY KEY, SessionType TEXT NOT NULL, Profile TEXT)");
    executeSqlStatement("CREATE TABLE Message(SessionID INT NOT NULL, MessageID INT NOT NULL, SenderUsername TEXT NOT NULL, MessageText TEXT NOT NULL, Profile TEXT, PRIMARY KEY(SessionID, MessageID), FOREIGN KEY(SessionID) REFERENCES Session(SessionID), FOREIGN KEY(SenderUserName) REFERENCES User(Username))");
    executeSqlStatement("CREATE TABLE IsMember(SessionID INT NOT NULL, Username TEXT NOT NULL, PRIMARY KEY (Username, SessionID),  FOREIGN KEY (Username) REFERENCES User(username), FOREIGN KEY (SessionID) REFERENCES Session(SessionID))");
}

bool DatabaseOperation::isDBExist(QString dbfilename) const {
    QFileInfo file(dbfilename);
    return file.exists();
}

void DatabaseOperation::closeDB() {
    if (status != Status::Running) throw "already closed";
    database.close();

    status = Status::Stop;
    emit signal_DBstop();
}

QList<OnlineUserModel*> DatabaseOperation::findAllUsers() {
    QList<OnlineUserModel *> ret;
    QSqlQuery sqlQuery;
    sqlQuery.exec("SELECT Username, Nickname, Password, Profile FROM User");
    if(!sqlQuery.exec()) {
        qDebug() << "Error: Fail to query table. " << sqlQuery.lastError();
        throw "DB read error";
    }
    else {
        auto & dcenter = ClientDataCenter::Singleton();
        while(sqlQuery.next()) {
            QString username = sqlQuery.value(0).toString();
            QString nickname = sqlQuery.value(1).toString();
            QString profile = sqlQuery.value(3).toString();
            auto newuser = new OnlineUserModel(username, nickname, str2json(profile));
            dcenter.registerUser(newuser);
            ret.append(newuser);
        }
    }
    return ret;
}

QList<OnlineSession*> DatabaseOperation::findAllSessions() {
    QList<OnlineSession*> ret;
    QSqlQuery query;
    if (!query.exec("SELECT SessionID, Profile FROM Session")) {
        qDebug() << "findAllSessions: " << query.lastError();
        throw "DB read error";
    }
    auto & dcenter = ClientDataCenter::Singleton();
    while (query.next()) {
        int sessionId = query.value(0).toInt();
        QJsonObject json = query.value(1).toJsonObject();
        QString sessionName = json.contains("SessionName") ? json["SessionName"].toString() : "None";
        QList<QString> members = queryMembersBySession(sessionId);

        OnlineSession * session = new OnlineSession(sessionId, sessionName, json, members);
        ret.append(session);
        dcenter.registerSession(session);
    }
    return ret;
}

QList<OnlineMessage *> DatabaseOperation::findAllMessages() {
    QList<OnlineMessage *> ret;
    QSqlQuery query;
    if (!query.exec("SELECT SessionID, MessageID, senderUsername, MessageText, Profile FROM messages")) {
        while (query.next()) {
            int sessionID = query.value(0).toInt();
            int msgId = query.value(1).toInt();
            QString sender = query.value(2).toString();
            QString text = query.value(3).toString();
            QJsonObject profile = query.value(4).toJsonObject();

            auto * msg = new OnlineMessage(msgId, sessionID, sender, text, profile);
            ret.append(msg);
            ClientDataCenter::Singleton().registerMessage(msg);
        }
    }
    return ret;
}


//往User表中插入数据
bool DatabaseOperation::insertUser(QString username, QString nickname, QString password, QJsonObject profile){
    QSqlQuery query;
    QString insert_sql = "INSERT INTO User(Username, Nickname, Password, Profile) VALUES (?, ?, ?, ?)";
    query.prepare(insert_sql);
    query.addBindValue(username);
    query.addBindValue(nickname);
    query.addBindValue(password);
    query.addBindValue(json2str(profile));
    if (! query.exec(insert_sql) ) {
        qDebug() << query.lastError();
        return false;
    }
    ClientDataCenter::Singleton().registerUser(new OnlineUserModel(username, nickname, profile));
    return true;
}

bool DatabaseOperation::insertUser(const OnlineUserModel &user, const QString &password) {
    return insertUser(user.getUsername(), user.getNickname(), password, user.getProfile());
}



int DatabaseOperation::getTableCount(QString tableName) const {
    QSqlQuery query;
    QString sql = QString("SELECT COUNT(*) FROM %1").arg(tableName);
    query.addBindValue(tableName);
    if (!query.exec(sql)) {
        qDebug() << query.lastError();
        return -1;
    }
    if (!query.next()) return -1;
    return query.value(0).toInt();
}

//往 Session 表中插入数据
int DatabaseOperation::insertSessionBasicInfo(QString SessionType, QJsonObject profile) {
    QSqlQuery query;
    QString insert_sql = "insert into Session(SessionID, SessionType, Profile) values(?, ?, ?)";
    query.prepare(insert_sql);
    int sessionId = getTableCount("Session") + 1;
    query.addBindValue(sessionId);
    query.addBindValue(SessionType);
    query.addBindValue(json2str(profile));
    if (!query.exec()) {
        qDebug() << query.lastError();
        return -1;
    }
    return sessionId;
}

bool DatabaseOperation::insertMember(int sessionID, QString user){
    QSqlQuery query;
    QString insert_sql = "insert into IsMember values(?, ?)";
    query.prepare(insert_sql);
    query.addBindValue(sessionID);
    query.addBindValue(user);
    if(!query.exec()){
        qDebug()<<"query error: "<<query.lastError();
        return false;
    }
    return true;
}

bool DatabaseOperation::insertMember(int sessionID, const OnlineUserModel &user){
    return insertMember(sessionID, user.getUsername());
}

//通过sessionID查询会议成员
QList<QString> DatabaseOperation::queryMembersBySession(int sessionID){
    QList<QString> member_List;
    QSqlQuery query;
    QString query_sql = "SELECT SessionID, Username FROM IsMember WHERE sessionID = (?)";
    query.prepare(query_sql);
    query.addBindValue(sessionID);
    if (! query.exec()) {
        qDebug() << "error occurred in queryMembersBySession, " << query.lastError();
        return member_List;
    }
    while(query.next()){
        member_List.append(query.value(1).toString());
    }
    return member_List;
}

//查询用户所拥有的session
QList<int> DatabaseOperation::querySessionsByMember(QString username){
    QList<int> member_List;
    QSqlQuery query;
    QString query_sql = "SELECT SessionId, Username FROM IsMember WHERE username = (?)";
    query.prepare(query_sql);
    query.addBindValue(username);
    query.exec();
    while(query.next()){
        member_List.append(query.value(0).toInt());
    }
    return member_List;
}

bool DatabaseOperation::attemptLogIn(QString username, QString password) const {
    //用户名检测
    QSqlQuery query(database);
    query.prepare("select username, password from User where username=?");
    query.addBindValue(username);
    bool ok = query.exec();
    if(!ok){
        qDebug() << "Fail query register username" << query.lastError();
        return false;
    }
    if(query.next()){
        //密码检测
        if (query.value(1).toString() == password)
            return true;
        qDebug() << "password incorrect";
        return false;
    }
    qDebug() << "Username not found";
    return false;
}

int DatabaseOperation::insertNewMessage(int SessionId, QString senderUsername, QString MessageText, QJsonObject profile) {
    QSqlQuery query;
    QString sql = "select count (*) from Message WHERE SessionId = ?";
    query.prepare(sql);
    query.addBindValue(SessionId);
    if (!query.exec() || !query.next()) {
        qDebug() << "Error Occurred when querying Message Number" << query.lastError();
        return -1;
    }
    int msgId = query.value(0).toInt() + 1;
    qDebug() << "Current MsgId for sessionId = " << msgId;

    sql = "insert into Message(SessionID, MessageID, SenderUsername, MessageText, Profile) VALUES (?, ?, ?, ?, ?)";
    query.prepare(sql);
    query.addBindValue(SessionId);
    query.addBindValue(msgId);
    query.addBindValue(senderUsername);
    query.addBindValue(MessageText);
    query.addBindValue(json2str(profile));
    if (!query.exec()) {
        qDebug() << "insertNewMessage : " << query.lastError();
        return -1;
    }
    return msgId;
}

QList<int> DatabaseOperation::getMessageListBySessionID(int SessionId) const {
    QList<int> ret;
    QSqlQuery query;
    QString sql = "SELECT MessageID, SenderUsername, MessageText, Profile FROM Message WHERE SessionID = ?";
    query.prepare(sql);
    query.addBindValue(SessionId);
    if (!query.exec()) {
        qDebug() << "getMessageListBySessionID: " << query.lastError();
        throw query.lastError();
    }
    while(query.next()) {
        int msgId = query.value(0).toInt();
        QString senderUsername = query.value(1).toString();
        QString messageText = query.value(2).toString();
        QJsonObject profile = query.value(3).toJsonObject();
        auto * msg = new OnlineMessage(msgId, SessionId, senderUsername, messageText, profile);
        ret.append(msgId);
        ClientDataCenter::Singleton().registerMessage(msg);
    }
    return ret;
}

OnlineUserModel & ClientDataCenter::getUser(QString username) {
    if (_getUser(username) == nullptr) throw "Not exist";
    return *users[username];
}

OnlineMessage & ClientDataCenter::getMessage(int SessionId, int MessageId) {
    if (_getMessage(SessionId, MessageId) == nullptr) throw "Not exist";
    return *messages[{SessionId, MessageId}];
}

OnlineSession & ClientDataCenter::getSession(int SessionId) {
    if (_getSession(SessionId) == nullptr) throw "Not exist";
    return *sessions[SessionId];
}

OnlineUserModel* ClientDataCenter::_getUser(QString username) {
    qDebug() << "test" << users;
    qDebug() << "test" << username;
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

OnlineUserModel * DatabaseOperation::findUser(QString username) {
    QSqlQuery query;
    QString sql = "SELECT Username, Nickname, Profile FROM User WHERE Username = " + username;
    if (!query.exec(sql) || !query.first()) {
        qDebug() << "DBOps::findUser: " << query.lastError();
        return nullptr;
    }
    OnlineUserModel * ret = new OnlineUserModel(query.value(0).toString(),
                                                query.value(1).toString(),
                                                query.value(2).toJsonObject());
    ClientDataCenter::Singleton().registerUser(ret);
    return ret;
}

OnlineSession * DatabaseOperation::findSession(int sessionID) {
    QSqlQuery query;
    QString sql = "SELECT Profile FROM Session WHERE SessionID = " + QString::number(sessionID);
    if (!query.exec(sql) || !query.first()) {
        qDebug() << "DBOps::findSession: " << query.lastError();
        return nullptr;
    }

    auto json = query.value(0).toJsonObject();
    QString SessionName = json.contains("SessionName") ?
                json["SessionName"].toString() : "None";

    OnlineSession * ret = new OnlineSession(sessionID, SessionName, json,
                                            queryMembersBySession(sessionID));
    ClientDataCenter::Singleton().registerSession(ret);
    return ret;
}

OnlineMessage * DatabaseOperation::findMessage(int sessionId, int MessageId) {
    QSqlQuery query;
    QString sql = "SELECT SenderUsername, MessageText, Profile FROM Message WHERE SessionID = " +
            QString::number(sessionId) + " and MessageID = " + QString::number(MessageId);
    if (!query.exec(sql) || !query.first()) {
        qDebug() << "DBOps::findMessage: " << query.lastError();
        return nullptr;
    }

    QString sender = query.value(0).toString();
    QString text = query.value(1).toString();
    QJsonObject json = query.value(2).toJsonObject();
    OnlineMessage * ret = new OnlineMessage(MessageId, sessionId, sender, text, json);
    ClientDataCenter::Singleton().registerMessage(ret);
    return ret;
}

ClientDataCenter::ClientDataCenter(QObject *parent) : QObject(parent)
{
    connect(&DatabaseOperation::Singleton(), &DatabaseOperation::signal_DBstop, this, &ClientDataCenter::clean);
}
