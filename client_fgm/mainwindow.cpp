#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "msgsession.h"
#include "clientdatacenter.h"
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>
#include <QToolButton>
#include <QCompleter>
#include <QWidgetAction>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setAttribute(Qt::WA_StyledBackground);

    QJsonObject data = {{ "MsgType", "SessionData" }, {"SessionID", 1}, {"SessionType", "FRIEND"} };
    QJsonArray members;
    members.append(QJsonObject{{"Username", "coocoo"}});
    members.append(QJsonObject{{"Username", "xxxxxx"}});
    data["Members"]=members;
    MsgSession *k = new MsgSession("coocoo", data, this);
    friendlayout = new QVBoxLayout(ui->frd);
    friendlayout->setContentsMargins(0,0,0,0);
    friendlayout->addWidget(k);

    QToolButton* btnSearch = new QToolButton;
    btnSearch->setCursor(Qt::PointingHandCursor);//如果不设置鼠标样式，鼠标移动到按钮上依旧显示为编辑框的鼠标样式
    btnSearch->setIcon(QPixmap(":/img/system/img/search.png"));
    btnSearch->setIconSize(QSize(20,20));
    btnSearch->setStyleSheet("QToolButton{border:none;}");
    QWidgetAction* action = new QWidgetAction(ui->search);
    action->setDefaultWidget(btnSearch);
    ui->search->addAction(action, QLineEdit::TrailingPosition);

    ui->widget_2->hide();
    lblBackground = new QLabel(ui->widget_3);
    lblBackground->setPixmap(QPixmap(":/img/system/img/bicBackground.png"));
    lblBackground->setGeometry((this->width() - 323)/ 2 - 100, this->height() / 2 - 200, 250, 250);
    lblWelcome = new QLabel(ui->widget_3);
    lblWelcome->setText("Hello, Coder!");
    QFont fontWelcome("Verdana", 37, 90);
    lblWelcome->setFont(fontWelcome);
    lblWelcome->setStyleSheet("color:rgb(144,224,239);");
    lblWelcome->setGeometry((this->width() - 323)/ 2 - 200, this->height() / 2 + 50, 500, 100);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dealMessage(Message *messageW, QListWidgetItem *item, QString text, QString time, Message::UserType type)
{
    messageW->setFixedWidth(ui->listWidget->width() -10);//固定聊天气泡的长度 不能超过父窗口
    QSize size = messageW->setSize(text);                //设置message基本规格
    item->setSizeHint(size);                             //设置item基本规格 同widget
    messageW->setTextContent(text, time, size, type);    //设置message基本内容
    ui->listWidget->setItemWidget(item, messageW);       //将message的内容 在给定的 item中显示

    ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
}

void MainWindow::dealMessageTime(QString curMsgTime)
{
    bool isShowTime = false;
    if(ui->listWidget->count() > 0) {
        QListWidgetItem* lastItem = ui->listWidget->item(ui->listWidget->count() - 1);
        Message* messageW = (Message*)ui->listWidget->itemWidget(lastItem);
        int lastTime = messageW->time().toInt();
        int curTime = curMsgTime.toInt();
        qDebug() << "curTime lastTime:" << curTime - lastTime;
        isShowTime = ((curTime - lastTime) > 60); // 两个消息相差一分钟
//        isShowTime = true;
    } else {
        isShowTime = true;
    }
    if(isShowTime) {
        Message* messageTime = new Message(ui->listWidget->parentWidget());
        QListWidgetItem* itemTime = new QListWidgetItem(ui->listWidget);

        QSize size = QSize(ui->listWidget->width() - 10, 40);
        messageTime->resize(size);
        itemTime->setSizeHint(size);
        messageTime->setTextContent(curMsgTime, curMsgTime, size, Message::userTime);
        ui->listWidget->setItemWidget(itemTime, messageTime);
    }
}

void MainWindow::on_btnSend_clicked()
{
    QString msg = ui->textEdit->toPlainText(); //返回文字
    ui->textEdit->setText("");                 //清空
    emit SendMessageToServer(username, msg);
    QList<QString> owners, repos, commits;
    //int git_mentions = captureGitRepo(msg, owners, repos, commits);
//    for (int i = 0; i < git_mentions; i++) {
//        //msg = generateGitMessage(owners[i], repos[i], commits[i]);
//        //emit SendMessageToServer(username, msg);
//    }

    ui->listWidget->setCurrentRow(ui->listWidget->count()-1);//保持在最新的Item
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    for(int i = 0; i < ui->listWidget->count(); i++) {
        Message* messageW = (Message*)ui->listWidget->itemWidget(ui->listWidget->item(i));
        QListWidgetItem* item = ui->listWidget->item(i);

        dealMessage(messageW, item, messageW->text(), messageW->time(), messageW->userType());
    }

    lblBackground->setGeometry((this->width() - 323)/ 2 - 100, this->height() / 2 - 200, 250, 250);
    lblWelcome->setGeometry((this->width() - 323)/ 2 - 200, this->height() / 2 + 50, 500, 100);
}


MainWindow::MainWindow(QJsonObject data,QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setAttribute(Qt::WA_StyledBackground);
    friendlayout = new QVBoxLayout(ui->frd);
    grouplayout = new QVBoxLayout(ui->group);
    friendlayout->setContentsMargins(0,0,0,0);
    grouplayout->setContentsMargins(0,0,0,0);
    searchlayout = new QVBoxLayout(ui->searchkuang);
    newgrouplayout = new QVBoxLayout(ui->newgroup);
    newgroupviewlayout = new QVBoxLayout(ui->newgroupview);
    setup(data);
    friendlayout->addStretch();
    grouplayout->addStretch();
    ui->widget_2->hide();
    lblBackground = new QLabel(ui->widget_3);
    lblBackground->setPixmap(QPixmap(":/img/system/img/bicBackground.png"));
    lblBackground->setGeometry((this->width() - 323)/ 2 - 100, this->height() / 2 - 200, 250, 250);
    lblWelcome = new QLabel(ui->widget_3);
    lblWelcome->setText("Hello, Coder!");
    QFont fontWelcome("Verdana", 33, 90);
    lblWelcome->setFont(fontWelcome);
    lblWelcome->setStyleSheet("color:rgb(1, 73, 124);");
    lblWelcome->setGeometry((this->width() - 323)/ 2 - 200, this->height() / 2 + 50, 500, 100);
}

void MainWindow::setup(QJsonObject data) {
    ui->userNameShow->setText(data["Username"].toString());
    username = data["Username"].toString();
    ui->nickNameShow->setText(data["Nickname"].toString());

    //搜索框按钮
    QToolButton* btnSearch = new QToolButton;
    btnSearch->setCursor(Qt::PointingHandCursor);//如果不设置鼠标样式，鼠标移动到按钮上依旧显示为编辑框的鼠标样式
    btnSearch->setIcon(QPixmap(":/img/system/img/search.png"));
    btnSearch->setIconSize(QSize(20,20));
    btnSearch->setStyleSheet("QToolButton{border:none;}");
    QWidgetAction* action = new QWidgetAction(ui->search);
    action->setDefaultWidget(btnSearch);
    ui->search->addAction(action, QLineEdit::TrailingPosition);
    connect(btnSearch,&QToolButton::clicked,[=](){
        QString msg = ui->search->text();
        emit SendSearchInfo(msg);
    });
}

void MainWindow::dealsearchinfo(QJsonObject data){
    QJsonArray arr = data["SearchInfo"].toArray();
    QString UserName,NickName;
    int n = kuangstore.size(),i;
    for(i=0;i<n;i++){
        searchlayout->removeWidget(kuangstore[i]);
        delete kuangstore[i];
    }
    kuangstore = {};
    n = arr.size();
    for(i=0;i<n;i++){
        UserName = arr[i].toObject()["Username"].toString();
        NickName = arr[i].toObject()["Nickname"].toString();
        MsgSession *k = new MsgSession(UserName,NickName);
        kuangstore.append(k);
        connect(k,&MsgSession::msgSessionChanged,this,&MainWindow::SendNewFriendInfo);
        searchlayout->addWidget(k);
    }
}
void MainWindow::SendNewFriendInfo(MsgSession *This){
    int j,n1 = friendsession.size();
    if(This->USERNAME == username){
        QMessageBox::information(this,"提示","不能添加自己为好友");
        return;
    }
    for(j=0;j < n1;j++){
        if(friendsession[j]->USERNAME == This->USERNAME){
            QMessageBox::information(this,"提示",friendsession[j]->USERNAME + "已经是您的好友了");
            return;
        }
    }
    if(This->IsChosen){
        QMessageBox::information(this,"提示","请勿重复发送好友请求");
    }
    else {
        QMessageBox::information(this,"提示","添加好友信息已发送");
        This->IsChosen = true;
        emit SendNewFriendInfo1(This);
    }
}

void MainWindow::showreddot(MsgSession *This){
    This->Showreddot();
}

void MainWindow::hidereddot(MsgSession *This){
    This->Hidereddot();
}

void MainWindow::Sessionorderchange(QJsonObject data){
    int ID = data["SessionID"].toInt(),i,n;
    n = friendsession.size();
    for(i=0;i<n;i++){
        if(friendsession[i]->SessionID == ID){
            friendlayout->removeWidget(friendsession[i]);
            friendlayout->insertWidget(0,friendsession[i]);
            if(data["SenderName"].toString()!=username){
                showreddot(friendsession[i]);
            }
            return;
        }
    }
    n = groupsession.size();
    for(i=0;i<n;i++){
        if(groupsession[i]->SessionID == ID){
            grouplayout->removeWidget(groupsession[i]);
            grouplayout->insertWidget(0,groupsession[i]);
            if(data["SenderName"].toString()!=username){
                showreddot(groupsession[i]);
            }
            return;
        }
    }
}

void MainWindow::FriendSessionAdd(QJsonObject data){
    QLayoutItem * lastItem = friendlayout->itemAt(friendlayout->count() - 1); // 头像弹簧
    friendlayout->removeItem(lastItem);
    MsgSession *k = new MsgSession(username,data,this);
    friendlayout->addWidget(k);
    friendsession.append(k);
    connect(k,&MsgSession::msgSessionChanged,this,&MainWindow::clearlistview);
    MsgSession *t = new MsgSession(username,data,this);
    newgrouplayout->addWidget(t);
    kuanggroupstore.append(t);
    connect(t,&MsgSession::msgSessionChanged,this,&MainWindow::groupchanged);
    friendlayout->addStretch();
}

void MainWindow::groupchanged(MsgSession *This){
    MsgSession::MsgSessionChosenNow = nullptr;
    if(This->IsChosen == true){
        newgroupviewlayout->removeWidget(This);
        newgrouplayout->addWidget(This);
        This->IsChosen = false;
    }
    else{
        newgrouplayout->removeWidget(This);
        newgroupviewlayout->addWidget(This);
        This->IsChosen = true;
    }
}

void MainWindow::GroupSessionAdd(QJsonObject data){
    QLayoutItem * lastItem = grouplayout->itemAt(grouplayout->count() - 1); // 头像弹簧
    grouplayout->removeItem(lastItem);
    MsgSession *k = new MsgSession(data,this);
    grouplayout->addWidget(k);
    groupsession.append(k);
    connect(k,&MsgSession::msgSessionChanged,this,&MainWindow::clearlistview);
    grouplayout->addStretch();
}

void MainWindow::clearlistview(MsgSession *This){
    ui->listWidget->clear();
    if(MsgSession::IsShow){
        MsgSession::IsShow = false;
        ui->widget_2->show();
        lblBackground->hide();
        lblWelcome->hide();
    }
    ui->lblUsername->setText(This->USERNAME);
    hidereddot(This);
    emit updatelistview();
}

void MainWindow::AddMessagetoListview(QJsonObject data){
    if(MsgSession::MsgSessionChosenNow && data["SessionID"].toInt()==MsgSession::MsgSessionChosenNow->SessionID){
        hidereddot(MsgSession::MsgSessionChosenNow);
        QString time = QString::number(QDateTime::currentDateTime().toTime_t());
        dealMessageTime(time);                     //用于处理时间
        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
        QString msg = data["Body"].toObject()["Text"].toString();
        if(data["SenderName"].toString() == username){
            Message *messageW = new Message(username,ui->listWidget->parentWidget());
            dealMessage(messageW, item, msg, time, Message::userMe);
        }
        else{
            Message *messageW = new Message(data["SenderName"].toString(),ui->listWidget->parentWidget());
            dealMessage(messageW, item, msg, time, Message::userOther);
        }
    }
}

void MainWindow::on_sendgroup_clicked()
{
    QString msg = ui->lineEdit->text();
    if(msg.size()==0){
        QMessageBox::information(this,"提示","请为您的群聊设置一个昵称");
        return;
    }
    QJsonArray temp;
    int i, n = kuanggroupstore.size();
    for(i=0;i<n;i++){
        if(kuanggroupstore[i]->IsChosen){
            temp.append(QJsonObject({{"username", kuanggroupstore[i]->USERNAME}}));
        }
    }
    temp.append(QJsonObject({{"username", username}}));
    QJsonObject data = {{"MsgType","NewGroup"},{"SessionName",msg}};
    data["Members"] = temp;
    emit sendnewgroup(data);
}
