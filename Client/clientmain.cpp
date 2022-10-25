#include "clientmain.h"
#include <QHostAddress>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMessageBox>
#include <QCoreApplication>
#include <QtScript>

ClientMain::ClientMain(QString IPAddress, int portOpen, QObject* parent)
    : QObject(parent)
    , ipAdd(IPAddress)
    , port(portOpen)
{
    connectToServer();
    connect(socket, &QTcpSocket::connected, this, [=]() {
        emit serverConnected();
    });
    connect(socket, &QTcpSocket::disconnected, this, [=]() {
        emit serverDisconnected();
    });
    //接受消息
    connect(socket, &QTcpSocket::readyRead, this, &ClientMain::receiveMessage);
}

void ClientMain::createmainwindow(QJsonObject data)
{
    main = new MainWindow(data);
    main->show();
    //从服务器收到message时尝试加入mainwindow
    connect(this, &ClientMain::SessionMessageReceived, main, &MainWindow::AddMessagetoListview);

    //向对话列表中添加窗口
    connect(&datacenter, &ClientDataCenter::FriendSessionDataReceived, main, &MainWindow::FriendSessionAdd);
    connect(&datacenter, &ClientDataCenter::GroupSessionDataReceived, main, &MainWindow::GroupSessionAdd);

    //当目前选择的窗口改变时检索当前选择窗口的message并加入mainwindow
    connect(main, &MainWindow::updatelistview, &datacenter, &ClientDataCenter::GetSessionMessage);
    connect(&datacenter, &ClientDataCenter::addsessionmessage, main, &MainWindow::AddMessagetoListview);

    //send按钮点击后发送该消息到服务器
    connect(main, &MainWindow::SendMessageToServer, this, &ClientMain::MessageFromMainwindow);
    connect(main, &MainWindow::charMsg, this, &ClientMain::MessageFromMainwindow1);
    connect(main, &MainWindow::sendfile1, this, &ClientMain::MessageFromMainwindow2);

    connect(main, &MainWindow::touXiang, this, &ClientMain::sendtouxiangmessage);
    connect(main, &MainWindow::signatureChange, this, &ClientMain::Signaturesend);
    //搜索按钮点击后发送消息到server
    connect(main, &MainWindow::SendSearchInfo, this, &ClientMain::SendSearchInfoToSever);
    connect(this, &ClientMain::SearchInfoReceived, main, &MainWindow::dealsearchinfo);
    connect(main, &MainWindow::SendNewFriendInfo1, this, &ClientMain::SendNewFriendInfoToSever);
    connect(main, &MainWindow::sendnewgroup, this, &ClientMain::send);
    connect(&datacenter, &ClientDataCenter::sessionorderchange, main, &MainWindow::Sessionorderchange);
}
void ClientMain::Signaturesend(QJsonObject data)
{
    send(data);
}

void ClientMain::sendtouxiangmessage(QString username1, QString filename)
{
    QJsonObject data = { { "MsgType", "huantouRequest" }, { "user", username1 }, { "url", filename } };
    send(data);
}

void ClientMain::SendNewFriendInfoToSever(Kuang* This)
{
    Kuang::KuangChosenNow = nullptr;
    QJsonObject data = { { "MsgType", "AddFriendRequest" }, { "FromUsername", main->username }, { "ToUsername", This->USERNAME } };
    send(data);
}

void ClientMain::MessageFromMainwindow(const QString& sendername, const QString& text)
{
    OnlineSession& temp = datacenter.getSession(Kuang::KuangChosenNow->SessionID);
    QJsonObject data = { { "MsgType", "SessionMessage" },
        { "SessionID", temp.getSessionID() },
        { "SenderName", sendername },
        { "MessageID", -1 } };
    QJsonObject Profile = {
        { "hasMentionInfo", false },
        { "ReadMark", false }
    };
    Profile = { { "Text", text }, { "Profile", Profile } };
    data["Body"] = Profile;
    send(data);
}
void ClientMain::MessageFromMainwindow2(QString sendername, QJsonObject text)
{
    OnlineSession& temp = datacenter.getSession(Kuang::KuangChosenNow->SessionID);
    QJsonObject data = { { "MsgType", "fileSessionMsg" },
        { "SessionID", temp.getSessionID() },
        { "SenderName", sendername },
        { "MessageID", -1 } };
    QJsonObject Profile = {
        { "hasMentionInfo", false },
        { "ReadMark", false }
    };
    Profile = { { "Text", "file message" }, { "Picture/file", text }, { "Profile", Profile } };
    data["Body"] = Profile;
    qDebug() << "send -----------" << data;
    send(data);
}

void ClientMain::MessageFromMainwindow1(QString sendername, QString text)
{
    qDebug() << "began pkg" << sendername << text;
    OnlineSession& temp = datacenter.getSession(Kuang::KuangChosenNow->SessionID);
    QJsonObject data = { { "MsgType", "SessionMessage" },
        { "SessionID", temp.getSessionID() },
        { "SenderName", sendername },
        { "MessageID", -1 } };
    QJsonObject Profile = {
        { "hasMentionInfo", false },
        { "ReadMark", false }
    };
    Profile = { { "Text", "file/picture message" }, { "url", text }, { "Profile", Profile } };
    data["Body"] = Profile;
    qDebug() << data;
    send(data);
}

void ClientMain::send(QJsonObject data)
{
    QString str = QString(QJsonDocument(data).toJson());
    qDebug() << "### send " << str;
    socket->write(str.toUtf8());
}

void ClientMain::connectToServer()
{
    socket = new QTcpSocket();
    //连接信息提示
    connect(socket, &QTcpSocket::connected, this, [=]() {
        qDebug() << "Server Connected";
        is_connected = true;
        emit serverConnected();
    });
    connect(socket, &QTcpSocket::disconnected, this, [=]() {
        qDebug() << "Server Disconnected";
        is_connected = false;
        emit serverDisconnected();
    });
    //接受消息
    connect(socket, &QTcpSocket::readyRead, this, &ClientMain::receiveMessage);
    socket->connectToHost(QHostAddress(ipAdd), port);
}

void ClientMain::processMethod(QJsonObject data)
{
    if (data["MsgType"].toString() == "UserData") {
        emit UserDataReceived(data);
    }
    if (data["MsgType"].toString() == "LogInConfirm") {
        emit LogInConfirmReceived(data);
    }
    if (data["MsgType"].toString() == "RegistConfirm") {
        emit RegistConfirmReceived(data);
    }
    if (data["MsgType"].toString() == "SessionMessage") {
        emit SessionMessageReceived(data);
    }
    if (data["MsgType"].toString() == "SessionData") {
        emit AddSessionToDatabase(data);
    }
    if (data["MsgType"].toString() == "SearchInfo") {
        emit SearchInfoReceived(data);
    }
    if (data["MsgType"].toString() == "fileSessionMsg") {
        emit SessionMessageReceived(data);
    }
    if (data["MsgType"].toString() == "AddFriendRequest") {
        //        emit ShowAddFriendRequestResult(data);
        if (data["Result"] == "succeeded")
            qDebug() << "弹出已申请好友的弹窗";
        else
            qDebug() << "对方已经是你的好友";
    }
    if (data["MsgType"].toString() == "AddFriendNewRequest") {
        qDebug() << "11111111!!!!!!!";
        emit ShowNewFriendRequest(data);
    }
    if (data["MsgType"].toString() == "AddFriendResult") {
        qDebug() << "AddFriendResult";
        emit ShowAddFriendResult(data);
    }
}

void ClientMain::SendSearchInfoToSever(QString msg)
{
    QJsonObject data = { { "MsgType", "Search" } };
    data["SearchInfo"] = msg;
    send(data);
}

void ClientMain::receiveMessage()
{
    QByteArray arr = socket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(arr);
    QJsonObject data = doc.object();
    qDebug() << "Received from server: " << data;
    if (!data.contains("MsgType"))
        return;
    if (data["MsgType"].toString() == "JsonArray") {
        auto array = data["MsgList"].toArray();
        for (int i = 0; i < array.size(); i++) {
            processMethod(array[i].toObject());
        }
    } else {
        processMethod(data);
    }
}

void ClientMain::operator()()
{
    login = new UserLogin();
    regist = new UserRegister();
    login->show();

    regist = new UserRegister();

    // register form
    connect(login, &UserLogin::registerButtonClicked, this, [=]() {
        regist->show();
    });

    connect(login, &UserLogin::createMainWindow, this, &ClientMain::createmainwindow);
    connect(regist, &UserRegister::registfinished, login, &UserLogin::show);
    connect(login, &UserLogin::sendlogindata, this, &ClientMain::send);
    connect(regist, &UserRegister::sendregistdata, this, &ClientMain::send);
    connect(this, &ClientMain::LogInConfirmReceived, login, &UserLogin::loginconfirm);
    connect(this, &ClientMain::RegistConfirmReceived, regist, &UserRegister::registconfirm);
    connect(this, &ClientMain::SessionMessageReceived, &datacenter, &ClientDataCenter::RegisterMessage);
    connect(this, &ClientMain::AddSessionToDatabase, &datacenter, &ClientDataCenter::RegisterSession);

    //gjy
    connect(this,&ClientMain::ShowNewFriendRequest,this,&ClientMain::NewFriendRequest);
    connect(this,&ClientMain::ShowAddFriendResult,this,&ClientMain::AddFriendResult);
}



//gjy

void ClientMain::NewFriendRequest(QJsonObject data)
{
    if(data["MsgType"].toString() == "AddFriendNewRequest")
    {
        qDebug() << "好友请求弹窗";
        {
            main->showNewFriendPop(data["FromUsername"].toString());

        }
        QJsonObject json;
        json["MsgType"] = "AddFriendResponse";
        json["FromUsername"] = data["FromUsername"];
        json["ToUsername"] = data["ToUsername"];
        json["Result"] = "succeeded";
        send(json);
    }
}

void ClientMain::AddFriendResult(QJsonObject data)
{
    qDebug() << "AddFriendResult";
    if(data["MsgType"].toString() == "AddFriendResult")
    {
        if(data["Result"].toString() == "failed")
        {
            qDebug() << "xxx拒绝了你的好友请求";
        }
        else
        {
            qDebug() << "xxx通过了你的好友请求";
        }
    }
}
