#include "clientsir.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QHostAddress>
#include <QMessageBox>

ClientSir::ClientSir(QString IPAddress, int port, QObject *parent) : QObject(parent),ipAddress_(IPAddress), port_(port){
    socket = new QTcpSocket();
    //连上和断开信息提示
    connect(socket, &QTcpSocket::connected, this, [=](){
        qDebug()<<"Server Conneted";
    });
    connect(socket, &QTcpSocket::disconnected, this, [=](){
        qDebug()<<"Server Disconnected";
    });
    //开始接受消息
    socket->connectToHost (QHostAddress(ipAddress_), port_);
    connect(socket, &QTcpSocket::readyRead, this, &ClientSir::receiveMessage);
}

//创建主窗口
void ClientSir::createmainwindow(QJsonObject data){
    mainw = new MainWindow(data);
    mainw->show();
    //将从服务器返回的信息加载到界面上
    connect(this, &ClientSir::SessionMessageReceived, mainw, &MainWindow::AddMessagetoListview);

    //向对话列表中添加窗口
    connect(&datacenter,&ClientDataCenter::FriendSessionDataReceived,mainw,&MainWindow::FriendSessionAdd);
    connect(&datacenter,&ClientDataCenter::GroupSessionDataReceived,mainw,&MainWindow::GroupSessionAdd);

    //当目前选择的窗口改变时检索当前选择窗口的message并加入mainwindow
    connect(mainw,&MainWindow::updatelistview,&datacenter,&ClientDataCenter::GetSessionMessage);
    connect(&datacenter,&ClientDataCenter::addsessionmessage,mainw,&MainWindow::AddMessagetoListview);

    //send按钮点击后发送该消息到服务器
    connect(mainw,&MainWindow::SendMessageToServer,this,&ClientSir::MessageFromMainwindow);

    //搜索按钮点击后发送消息到server
    connect(mainw,&MainWindow::SendSearchInfo,this,&ClientSir::SendSearchInfoToSever);
    //从服务器接收搜索结果，并处理
    connect(this,&ClientSir::SearchInfoReceived,mainw,&MainWindow::dealsearchinfo);
    //添加朋友
    connect(mainw,&MainWindow::SendNewFriendInfo1,this,&ClientSir::SendNewFriendInfoToSever);
    //新建群里
    connect(mainw,&MainWindow::sendnewgroup,this,&ClientSir::send);
    //数据中心会话窗口改变，致使主页面会话窗口改变
    connect(&datacenter,&ClientDataCenter::sessionorderchange,mainw,&MainWindow::Sessionorderchange);

}



//发送添加朋友请求到服务器
void ClientSir::SendNewFriendInfoToSever(MsgSession *This){
    MsgSession::MsgSessionChosenNow = nullptr;
    QJsonObject data = {{"MsgType","AddFriendRequest"}, {"FromUsername", mainw->username}, {"ToUsername", This->USERNAME}};
    send(data);
}


//将当前聊天窗内要发送的信息打包发至服务器
void ClientSir::MessageFromMainwindow(const QString &sendername,const QString &text){
    OnlineSession &temp = datacenter.getSession(MsgSession::MsgSessionChosenNow->SessionID);
    QJsonObject data = {{"MsgType","SessionMessage"},
                        {"SessionID",temp.getSessionID()},
                        {"SenderName",sendername},
                        {"MessageID", -1}
                       };
    QJsonObject Profile = {
        {"hasMentionInfo",false},
        {"ReadMark",false}
      };
    Profile = {{"Text",text},{"Profile",Profile}};
    data["Body"] = Profile;
    send(data);
}


//发送搜索请求
void ClientSir::SendSearchInfoToSever(QString msg){
    QJsonObject data = {{"MsgType","Search"}};
    data["SearchInfo"] = msg;
    send(data);
}

//发送数据给服务器
void ClientSir::send(QJsonObject data){
    QString str = QString(QJsonDocument(data).toJson());
    qDebug()<< "### send "<<str;
    socket->write(str.toUtf8());
}


//客户端接收到消息，转接初步处理
void ClientSir::receiveMessage(){
    QByteArray arr = socket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(arr);
    QJsonObject data = doc.object();
    qDebug() << "Received from server: "<< data;
    if(!data.contains("MsgType")) return;
    if (data["MsgType"].toString() == "JsonArray") {
        auto array = data["MsgList"].toArray();
        for (int i = 0; i < array.size(); i++) {
            processMethod(array[i].toObject());
        }
    }else {
        processMethod(data);
    }
}

void ClientSir::processMethod(QJsonObject data){
    if(data["MsgType"].toString()=="UserData"){
        emit UserDataReceived(data);
    }else if(data["MsgType"].toString()=="LogInConfirm"){
        static bool init = false;                               //防止本地一个进程登录2次
        if (init) return;
        init = true;
        emit LogInConfirmReceived(data);
    }else if(data["MsgType"].toString()=="RegistConfirm"){
        emit RegistConfirmReceived(data);
    }else if(data["MsgType"].toString()=="SessionMessage"){
        emit SessionMessageReceived(data);
    }else if(data["MsgType"].toString()=="SessionData"){
        emit AddSessionToDatabase(data);
    }else if(data["MsgType"].toString()=="SearchInfo"){
        emit SearchInfoReceived(data);
    }
}



void ClientSir::operator()() {
    login = new Login();

    login->show();
    userregister = new UserRegister();
    // register form
    connect(login, &Login::registerButtonClicked, this, [=]() {
        userregister->show();
    });

    //启动client main时，构建connect列表

    //收到登录成功的消息后，login发出创建主页面申请，Client对象创建主页面
    connect(login, &Login::createMainWindow, this, &ClientSir::createmainwindow);
    //登录对象发出登录的请求，Client对象负责发送登录信息到服务器
    connect(login, &Login::sendlogindata, this, &ClientSir::send);
    //注册对象发出注册完成的信息后，登录界面显现
    connect(userregister, &UserRegister::registfinished, login, &Login::show);
    //注册对象发出注册信息合法且已填写，希望发送服务器的请求，Client发送服务器
    connect(userregister,&UserRegister::sendregistdata,this,&ClientSir::send);
    //Client收到服务器关于登录信息的反馈，返还给login
    connect(this,&ClientSir::LogInConfirmReceived,login,&Login::loginconfirm);
    //Clent收到服务器关于注册信息的反馈，返还给register
    connect(this,&ClientSir::RegistConfirmReceived,userregister,&UserRegister::registconfirm);
    //Client收到服务器的会话消息、添加会话信息到数据库，转到数据中心
    connect(this,&ClientSir::SessionMessageReceived,&datacenter,&ClientDataCenter::RegisterMessage);
    connect(this,&ClientSir::AddSessionToDatabase,&datacenter,&ClientDataCenter::RegisterSession);
}
