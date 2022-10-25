#include <QList>
#include <iostream>
#include <QJsonArray>
#include <QTime>
#include <QtGlobal>
#include <QDebug>
#include <QRegExp>

#include "databaseoperation.h"
#include "serverdatacenter.h"
#include "operations.h"

#include "ltest.h"

using namespace std;

TESTSUITE(BasicTest)
CASE(BasicOperations)
{
    assertTrue(1 + 1 == 2);
}

void raiseError() {
    throw "A Error";
}

CASE(AssertExceptionWork) {
    assertException( raiseError(); );
}

void execute() {
    EXE(BasicOperations);
    EXE(AssertExceptionWork);
}

ENDSUITE(BasicTest)

QObject obj;

TESTSUITE(SessionTest)

void ctor() {
    cout << "Test" << endl;
}

CASE(CreatingASessionOutOfListOfTwoUser) {
    QList<UserModel*> users;
    users.append(new OfflineUserModel("UserA")); users.append(new OfflineUserModel("UserB"));
    OfflineSession session (users);
    assertEqual(session.getSessionType(), AbstractSession::SessionType::FRIEND);
}

CASE(CreatingASessionOutOfListOfThreeUser) {
    QList<UserModel*> users;
    users.append(new OfflineUserModel("UserA")); users.append(new OfflineUserModel("UserB"));
    users.append(new OfflineUserModel("UserC"));
    OfflineSession session (users);
    assertEqual(session.getSessionType(), AbstractSession::SessionType::GROUP);
}

CASE(CreatingLocalSessionWithJsonObject) {
    QJsonObject json, user;
    QJsonArray userlist;
    json["MsgType"] = "SessionData";
    json["SessionID"] = 101;
    json["SessionType"] = "FRIEND";

    user["Username"] = "UserA";
    userlist.append(user);
    user["Username"] = "UserB";
    userlist.append(user);
    json["Members"] = userlist;
    assertEqual(json["Members"].toArray().size(), 2);

    OnlineSession session(json);
    assertEqual(session.getSessionID(), 101);
    assertEqual(session.getSessionType(), AbstractSession::SessionType::FRIEND);
    assertEqual(session.getMemberCount(), 2);
}

void execute() {
    EXE(CreatingASessionOutOfListOfTwoUser);
    EXE(CreatingASessionOutOfListOfThreeUser);
    EXE(CreatingLocalSessionWithJsonObject);
}

ENDSUITE(SessionTest)

TESTSUITE(UserTest)

CASE(OfflineUserGenerateJsonObject) {
    OfflineUserModel user("userA");
    user.setNickname("nicknameA");
    user.setSigniture("None");
    auto json = user.generateUserModelJson();
    assertEqual(json["Username"].toString(), "userA");
    assertEqual(json["Nickname"].toString(), "nicknameA");

    auto profile = json["Profile"].toObject();
    assertEqual(profile["signiture"].toString(), "None");
}

CASE(NewOnlineUser_ReadJson_LoadProfile) {
    OfflineUserModel user("userA");
    user.setNickname("nicknameA");
    user.setSigniture("None");

    auto json = user.generateUserModelJson();

    OnlineUserModel newuser(json);

    assertEqual(newuser.getType(), UserModel::Type::Online);
    assertEqual(newuser.getUsername(), "userA");
    assertEqual(newuser.getNickname(), "nicknameA");
    assertEqual(newuser.getSigniture(), "None");
}

void execute() {
    EXE(OfflineUserGenerateJsonObject);
    EXE(NewOnlineUser_ReadJson_LoadProfile);
}

ENDSUITE(UserTest)

TESTSUITE(DataCenterTest)

CASE(CanGetSingleton) {
    assertNoException(
        ServerDataCenter::Singleton();
    );
}

CASE(getOnlineUserFromUsername) {
    OfflineUserModel user("userA");
    user.setNickname("nicknameA");
    user.setSigniture("None");
    auto json = user.generateUserModelJson();
    OnlineUserModel newuser(json);

    OfflineUserModel userB("userB");
    userB.setNickname("nicknameA");
    userB.setSigniture("None");
    json = userB.generateUserModelJson();
    OnlineUserModel newuserB(json);

    auto& dcenter = ServerDataCenter::Singleton();
    dcenter.registerUser(& newuser);
    dcenter.registerUser(& newuserB);

    assertEqual(dcenter.getUser("userA").getNickname(), "nicknameA");
    assertEqual(dcenter.getUser("userB").getSigniture(), "None");

    assertEqual(dcenter.getUser("userA").parent(), &dcenter);
    assertEqual(dcenter.getUser("userB").parent(), &dcenter);
}

void execute() {
    EXE(CanGetSingleton);
}

ENDSUITE(DataCenterTest)


TESTSUITE(MessageTest)

CASE(NewMessage_GeneratedBy_UserAndSessionObject)
{
    OfflineUserModel userA("userA");
    userA.setNickname("nicknameA");
    userA.setSigniture("None");
    auto json = userA.generateUserModelJson();
    OnlineUserModel userA_online(json);

    OfflineUserModel userB("userB");
    userB.setNickname("nicknameA");
    userB.setSigniture("None");
    json = userB.generateUserModelJson();
    OnlineUserModel userB_online(json);

    ServerDataCenter & dcenter = ServerDataCenter::Singleton();
    dcenter.registerUser(&userA_online);
    dcenter.registerUser(&userB_online);

    QJsonObject user;
    QJsonArray userlist;
    json["MsgType"] = "SessionData";
    json["SessionID"] = 101;
    json["SessionType"] = "FRIEND";

    user["Username"] = "UserA";
    userlist.append(user);
    user["Username"] = "UserB";
    userlist.append(user);
    json["Members"] = userlist;
    assertEqual(json["Members"].toArray().size(), 2);

    OnlineSession session(json);

    testlog("Constructed mock users and session.");

    MessageModel msg(userA_online, session, "a->b text", QJsonObject());

    testlog("Generated offline message");
    assertEqual(msg.getType(), MessageModel::Type::Offline);
    assertEqual(msg.getMessageText(), "a->b text");
}

void execute() {
    EXE(NewMessage_GeneratedBy_UserAndSessionObject);
}
ENDSUITE(MessageTest)

QString randDBname;

TESTSUITE(DB)
DatabaseOperation& db = DatabaseOperation::Singleton();
QString dbname, fixed_dbname = "test.db";

void ctor() override {
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    randDBname = dbname = QString::number(qrand()) + ".db";
    testlog("dbname = %s (testlog)", dbname.toUtf8().data());
}

CASE(CanCreateNewDB) {
    assertTrue(!db.isDBExist(dbname));
    db.startDatabaseConnection(dbname);
    assertTrue(db.isDBExist(dbname));
    db.closeDB();
    assertTrue(!db.isRunning());
}

void insertUser() {
//    db.insertUser("xiaoming","XiaoMing", "xiaoming123", "{\"signiture\":\"nop\"}");
//    db.insertUser("xxx", "XXX", "xXxXx", "{ \"signiture\": \"yyy\"}");
//    db.insertUser("wuzirui", "wuzirui", "1234", "{ \"signiture\": \"cai\"}");
//    db.insertUser("usera", "UserA", "AAAA", "{ \"signiture\": \"B -1\"}");
//    db.insertUser("userB", "bibibi", "bbbb", "{ \"signiture\": \"A+1\"}");
}

void checkInsertedUser() {
    auto list = db.findAllUsers();
    assertEqual(db.getTableCount("User"), 5, db.getTableCount("User"));
    assertEqual(list.size(), 5);
    assertEqual(list.at(0)->getUsername(), "xiaoming");
    assertEqual(list.at(2)->getNickname(), "wuzirui");
    assertEqual(list.at(3)->getNickname(), "UserA");
    assertEqual(list.at(4)->getSigniture(), "A+1");
}

void insertSession() {
//    int startId = db.getTableCount("Session");
//    assertEqual(startId, 0);
//    int newId = db.insertSessionBasicInfo("FRIEND", "{}");
//    assertEqual(newId, startId + 1);
//    assertEqual(newId, db.getTableCount("Session"));

//    db.insertMember(newId, "wuzirui");
//    db.insertMember(newId, "xiaoming");
//    assertEqual(2, db.getTableCount("IsMember"),
//                "Expected 2, got " << db.getTableCount("IsMember"));
//    auto list = db.queryMembersBySession(newId);
//    assertEqual(list.size(), 2);
//    assertEqual(list.at(0), "wuzirui");
//    assertEqual(list.at(1), "xiaoming");
//    assertEqual(db.getTableCount("Session"), 1, db.getTableCount("Session"));

//    int newerId = db.insertSessionBasicInfo("GROUP", "{}");
//    assertEqual(newerId, newId + 1);
//    db.insertMember(newerId, "usera");
//    db.insertMember(newerId, "userB");
//    db.insertMember(newerId, "xxx");
//    db.insertMember(newerId, "wuzirui");
//    assertEqual(6, db.getTableCount("IsMember"), db.getTableCount("IsMember"));
//    list = db.queryMembersBySession(newerId);
//    assertEqual(list.size(), 4, list.size());
//    assertEqual(list.at(0), "usera", list.at(0).toUtf8().data());
//    assertEqual(list.at(1), "userB", list.at(1).toUtf8().data());
//    assertEqual(list.at(2), "xxx", list.at(2).toUtf8().data());
//    assertEqual(list.at(3), "wuzirui", list.at(3).toUtf8().data());

//    auto sslist = db.querySessionsByMember("wuzirui");
//    assertEqual(sslist.size(), 2);
//    assertEqual(sslist.at(0), newId, sslist.at(0));
//    assertEqual(sslist.at(1), newerId, sslist.at(1));
}

void insertMessage() {
//    int currentTextNum = db.getTableCount("Message");
//    assertEqual(currentTextNum, 0);
//    auto list = db.getMessageListBySessionID(2);
//    int currentTextId = list.size();
//    assertEqual(currentTextId, 0);
//    db.insertNewMessage(db.getTableCount("Session"),
//                        "wuzirui", ("This is a text, rand = %d"
//                        + QString::number(rand())).toUtf8().data(),
//                        "{\"hasMentionedInfo\": \"false\"}");
//    assertEqual(db.getTableCount("Message"),
//                currentTextNum + 1,
//                db.getTableCount("Message")
//                << " != " << currentTextNum + 1);
//    db.insertNewMessage(1, "xiaoming", "fixed text", "{}");
//    db.insertNewMessage(2, "wuzirui", "fixed text 2", "{}");
//    testlog("Inserted 3 messages");
//    list = db.getMessageListBySessionID(2);
//    assertEqual(2, list.size(), "a = " << 2 << ", b = " << list.size());
}


CASE(EmptyDB_InsertionTest) {
    db.startDatabaseConnection(dbname);
    insertUser();
    insertSession();
    insertMessage();
    checkInsertedUser();
    db.closeDB();
}

CASE(CanLoadExistingDB) {
    db.startDatabaseConnection(fixed_dbname);
    checkInsertedUser();
    db.closeDB();
}

CASE(CanLogIn_With_CorrectAccount) {
    assertTrue(db.attemptLogIn("wuzirui", "1234"));
    assertTrue(db.attemptLogIn("usera", "AAAA"));
}

CASE(LogIn_FailedTest) {
    assertTrue(!db.attemptLogIn("wuzirui", "wuzirui"));
    assertTrue(!db.attemptLogIn("bucunzai", "idontknow"));
    assertTrue(!db.attemptLogIn("UserA", "AAAA"));
}

void execute() override {
    EXE(CanCreateNewDB);
    EXE(EmptyDB_InsertionTest);
//    EXE(CanLoadExistingDB);
    EXE(CanLogIn_With_CorrectAccount);
    EXE(LogIn_FailedTest);
}

ENDSUITE(DB)


TESTSUITE(Functional)

DatabaseOperation &db = DatabaseOperation::Singleton();
ServerDataCenter &dcenter = ServerDataCenter::Singleton();
Operations &op = Operations::Singleton();

void ctor() {
    db.startDatabaseConnection(randDBname);
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
}
void dtor() { db.closeDB(); }

CASE(LoginProcessTest) {
    QJsonObject loginRequest;
    loginRequest["MsgType"] = "LogIn";
    loginRequest["Username"] = "wuzirui";
    loginRequest["Password"] = "1234";

    auto response = op.loginResponse(loginRequest);
    assertTrue(response.size() > 0);
    assertEqual(response.at(0)["MsgType"], "LogInConfirm");
    assertEqual(response.at(0)["IsLegal"].toBool(), true);

    loginRequest["Password"] = "4321";
    response = op.loginResponse(loginRequest);
    assertEqual(response.size(), 1);
    assertEqual(response.at(0)["IsLegal"].toBool(), false);
}

CASE(RegisterTest) {
    QString username = "User_" + QString::number(qrand());
    auto & dcenter = ServerDataCenter::Singleton();
    while (dcenter.hasUser(username)) {
        username = "User_" + QString::number(qrand());
    }
    testlog("register username %s", username.toUtf8().data());
    QString password = "upass::" + username;
    QString nickname = "tempuser";

    QJsonObject request = {{ "MsgType", "Regist" }};
    request["Username"] = username;
    request["Password"] = password;
    request["Nickname"] = nickname;

    auto response = op.registerResponse(request);
    assertEqual(response.size(), 1);
    assertEqual(response[0]["IsLegal"].toBool(), true);

    response = op.registerResponse(request);
    assertEqual(response.size(), 1);
    assertEqual(response[0]["IsLegal"].toBool(), false);
}

CASE(MessageForwardTest) {
    QString senderUsername = "wuzirui";
    QString text = "卷卷卷";
    int sessionID = 1;

    QJsonObject json = {{ "MsgID", "SessionMessage"}};
    json["SenderName"] = senderUsername;
    json["SessionID"] = sessionID;
    QJsonObject body;
    body["Text"] = text;
    body["Profile"] = QJsonObject({{"hasMentionInfo", false}});
    json["Body"] = body;

//    auto response = op.newMessageResponse(json);
}

void execute() {
    EXE(LoginProcessTest);
    for (int i = 0; i < 10; i++) {
        EXE(RegisterTest);
    }
    EXE(MessageForwardTest);
}

ENDSUITE(Functional)

int captureGitRepo(QString raw, QList<QString> &authors, QList<QString> &repos, QList<QString> &commits) {
    QRegExp reg("@\\[((?:[^ ,\\,])+),[ ]*((?:[^ ,\\,])+)(?:,[ ]*((?:[^ ,\\,])*))?\\]");
    int pos = 0, initial = authors.size();
    while ((pos = reg.indexIn(raw, pos)) != -1) {
        authors.append(reg.cap(1));
        repos.append(reg.cap(2));
        commits.append(reg.cap(3) == "" ? "master" : reg.cap(3));
        pos += reg.matchedLength();
    }
    return authors.size() - initial;
}

TESTSUITE(reg)

CASE(REG_Repo) {
    QList<QString> authors, repos, commits;
    int ret = captureGitRepo("a message with a gitee @[jasonliu233,zhishigongcheng, master] repo", authors, repos, commits);
    assertEqual(ret, 1, ret);
    testlog("Pattern recognized: author = %s, repo = %s, commit = %s", authors[0].toUtf8().data(),
            repos[0].toUtf8().data(), commits[0].toUtf8().data());
    assertEqual(authors[0], "jasonliu233");
    assertEqual(repos[0], "zhishigongcheng");
    assertEqual(commits[0], "master");
}


void execute() override {
    EXE(REG_Repo)
}
ENDSUITE(reg)
