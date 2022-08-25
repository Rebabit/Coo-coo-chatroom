#include "mainwindow.h"
#include "clientdatacenter.h"
#include "kuang.h"
#include "ui_mainwindow.h"
#include "emoji.h"
#include <QBuffer>
#include <QCompleter>
#include <QDateTime>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QHostAddress>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QToolButton>
#include <QWidgetAction>
#include <Session/addfriendrequestpop.h>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    //xyc
    setUpUi_xyc(); //设置按钮逻辑、透明背景、无边框拖拽的函数
    //xyc
    ui->tabWidget->setAttribute(Qt::WA_StyledBackground);

    QJsonObject data = { { "MsgType", "SessionData" }, { "SessionID", 1 }, { "SessionType", "FRIEND" } };
    QJsonArray members;
    members.append(QJsonObject { { "Username", "小蓝" } });
    members.append(QJsonObject { { "Username", "xxx" } });
    data["Members"] = members;
    Kuang* k = new Kuang("小蓝", data, this);
    friendlayout = new QVBoxLayout(ui->frd);
    friendlayout->setContentsMargins(0, 0, 0, 0);
    friendlayout->addWidget(k);

//    QToolButton* btnSearch = new QToolButton;
//    btnSearch->setCursor(Qt::PointingHandCursor); //如果不设置鼠标样式，鼠标移动到按钮上依旧显示为编辑框的鼠标样式
//    btnSearch->setIcon(QPixmap(":/img/system/img/search.png"));
//    btnSearch->setIconSize(QSize(20, 20));
//    btnSearch->setStyleSheet("QToolButton{border:none;}");
//    QWidgetAction* action = new QWidgetAction(ui->search);
//    action->setDefaultWidget(btnSearch);
//    ui->search->addAction(action, QLineEdit::TrailingPosition);

    ui->widget_2->hide();
    //    lblBackground = new QLabel(ui->widget_3);
    //    lblBackground->setPixmap(QPixmap(":/img/system/img/bicBackground.png"));
    //    lblBackground->setGeometry((this->width() - 323)/ 2 - 100, this->height() / 2 - 200, 250, 250);
    //    lblWelcome = new QLabel(ui->widget_3);
    //    lblWelcome->setText("Hello, Coder!");
    QFont fontWelcome("Verdana", 37, 90);
    //    lblWelcome->setFont(fontWelcome);
    //    lblWelcome->setStyleSheet("color:rgb(144,224,239);");
    //    lblWelcome->setGeometry((this->width() - 323)/ 2 - 200, this->height() / 2 + 50, 500, 100);
    //    ui->widget_2->show();
    //    lblBackground->hide();
    //    lblWelcome->hide();

    //connect(ui->sendfile, &QPushButton::clicked, this, &MainWindow::on_sendfile_cliked);
    //    connect(&myTimer, &QTimer::timeout, this, [=](){
    //        myTimer.stop();
    //        sendFile();
    //    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dealMessage(Message* messageW, QListWidgetItem* item, QString text, QString time, Message::UserType type, QString url)
{
    messageW->setFixedWidth(ui->listWidget->width() - 10); //固定聊天气泡的长度 不能超过父窗口
    QSize size = messageW->setSize(text); //设置message基本规格
    item->setSizeHint(size); //设置item基本规格 同widget
    messageW->setTextContent(text, time, size, type, url); //设置message基本内容
    ui->listWidget->setItemWidget(item, messageW); //将message的内容 在给定的 item中显示
    ui->listWidget->setCurrentRow(ui->listWidget->count() - 1);
}
void MainWindow::dealMessage1(Message *messageW, QListWidgetItem *item, QString text, QString file, QString time, Message::UserType type)
{
//    qDebug() << text;
//    qDebug() << file;
//    messageW->setFixedWidth(ui->listWidget->width() -10);//固定聊天气泡的长度 不能超过父窗口
//    QSize size = messageW->setSize(text);                //设置message基本规格
//    item->setSizeHint(size);                             //设置item基本规格 同widget
//    messageW->setTextContent(text, time, size, type);    //设置message基本内容
//    ui->listWidget->setItemWidget(item, messageW);       //将message的内容 在给定的 item中显示
//    ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
    messageW->setFixedWidth(ui->listWidget->width() -10);//固定聊天气泡的长度 不能超过父窗口
    QSize size = messageW->setSize(text);                //设置message基本规格
    item->setSizeHint(size);                             //设置item基本规格 同widget
    messageW->setTextContent(text, time, size, type);    //设置message基本内容
    ui->listWidget->setItemWidget(item, messageW);       //将message的内容 在给定的 item中显示
    ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
//    QImage image;
//    ui->listWidget->setViewMode(QListWidget::IconMode);

//    ui->listWidget->setIconSize(QSize(200,200));

//    ui->listWidget->setResizeMode(QListWidget::Adjust);

//    ui->listWidget->addItem(new QListWidgetItem(QIcon(file),messageW->MessageName));
//    ui->listWidget->setCurrentRow(ui->listWidget->count()-1);

//    image.load(file);
//    image.save(QString("./" + QDateTime::currentDateTime().toString("yyyyMMddhhmmsddd")+".png"), "png");
//    QLabel* labelImage = new QLabel(this, Qt::Dialog | Qt::WindowCloseButtonHint);
//    labelImage->setWindowTitle("Picture");
//    labelImage->resize(300, 300);
//    labelImage->setPixmap(QPixmap::fromImage(image.scaled(300, 300, Qt::KeepAspectRatio)));
//    labelImage->show();
}


void MainWindow::dealMessageTime(QString curMsgTime)
{
    bool isShowTime = false;
    if (ui->listWidget->count() > 0) {
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
    if (isShowTime) {
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
    QString msg = ui->textEdit->text(); //返回文字
    ui->textEdit->setText(""); //清空
    emit SendMessageToServer(username, msg);
    QList<QString> owners, repos, commits;
    //    int git_mentions = captureGitRepo(msg, owners, repos, commits);
    //    for (int i = 0; i < git_mentions; i++) {
    //        msg = generateGitMessage(owners[i], repos[i], commits[i]);
    //        emit SendMessageToServer(username, msg);
    //    }

    ui->listWidget->setCurrentRow(ui->listWidget->count() - 1); //保持在最新的Item
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    for (int i = 0; i < ui->listWidget->count(); i++) {
        Message* messageW = (Message*)ui->listWidget->itemWidget(ui->listWidget->item(i));
        QListWidgetItem* item = ui->listWidget->item(i);
        if (messageW->text() == "file/picture message")
        {
            qDebug() << "===picture===\n";
            //参数不知道啥——dyx
            dealMessage1(messageW, item, messageW->text(), messageW->fileURL, messageW->time(), messageW->userType());
        }
        else
            dealMessage(messageW, item, messageW->text(), messageW->time(), messageW->userType(), "Nop");
    }

    //    lblBackground->setGeometry((this->width() - 323)/ 2 - 100, this->height() / 2 - 200, 250, 250);
    //    lblWelcome->setGeometry((this->width() - 323)/ 2 - 200, this->height() / 2 + 50, 500, 100);
}
//bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
//用于光标焦点改变
//    if(event->type() == QEvent::MouseButtonPress && watched != ui->nickNameShow)
//    {
//         ui->nickNameShow->clearFocus();
//         this->setFocus();
//    }

//    return QMainWindow::eventFilter(watched, event);
//}

MainWindow::MainWindow(QJsonObject data, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setUpUi_xyc();
    ui->tabWidget->setAttribute(Qt::WA_StyledBackground);
    friendlayout = new QVBoxLayout(ui->frd);
    grouplayout = new QVBoxLayout(ui->group);
    friendlayout->setContentsMargins(0, 0, 0, 0);
    grouplayout->setContentsMargins(0, 0, 0, 0);
    searchlayout = new QVBoxLayout(ui->searchkuang);
    newgrouplayout = new QVBoxLayout(ui->newgroup);
    newgroupviewlayout = new QVBoxLayout(ui->newgroupview);
    setup(data);
    friendlayout->addStretch();
    grouplayout->addStretch();
    ui->widget_2->hide();
    //    lblBackground = new QLabel(ui->widget_3);
    //    lblBackground->setPixmap(QPixmap(":/img/system/img/bicBackground.png"));
    //    lblBackground->setGeometry((this->width() - 323)/ 2 - 100, this->height() / 2 - 200, 250, 250);
    //    lblWelcome = new QLabel(ui->widget_3);
    //    lblWelcome->setText("Hello, Coder!");
    QFont fontWelcome("Verdana", 33, 90);
    //    lblWelcome->setFont(fontWelcome);
    //    lblWelcome->setStyleSheet("color:rgb(1, 73, 124);");
    //    lblWelcome->setGeometry((this->width() - 323)/ 2 - 200, this->height() / 2 + 50, 500, 100);
    //
}

void MainWindow::setup(QJsonObject data)
{
    //    qDebug() <<"ssssssssssssssssss" << data;
    ui->userNameShow->setText(data["Username"].toString());
    username = data["Username"].toString();
    ui->nickNameShow->setText(data["Nickname"].toString());
    //    qDebug() << data["Profile"].toObject();
    //    qDebug() << data["Profile"].toObject()["signiture"].toString();
    ui->Signature->setText(data["Profile"].toObject()["signiture"].toString());
    //搜索框按钮
//    QToolButton* btnSearch = new QToolButton;
//    btnSearch->setCursor(Qt::PointingHandCursor); //如果不设置鼠标样式，鼠标移动到按钮上依旧显示为编辑框的鼠标样式
//    btnSearch->setIcon(QPixmap(":/img/system/img/search.png"));
//    btnSearch->setIconSize(QSize(20, 20));
//    btnSearch->setStyleSheet("QToolButton{border:none;}");
//    QWidgetAction* action = new QWidgetAction(ui->search);
//    action->setDefaultWidget(btnSearch);
//    ui->search->addAction(action, QLineEdit::TrailingPosition);
    connect(ui->UnConnect_pushButton_search_2, &QToolButton::clicked, [=]() {
        QString msg = ui->search->text();
        emit SendSearchInfo(msg);
    });
}

void MainWindow::dealsearchinfo(QJsonObject data)
{
    QJsonArray arr = data["SearchInfo"].toArray();
    QString UserName, NickName;
    int n = kuangstore.size(), i;
    for (i = 0; i < n; i++) {
        searchlayout->removeWidget(kuangstore[i]);
        delete kuangstore[i];
    }
    kuangstore = {};
    n = arr.size();
    for (i = 0; i < n; i++) {
        UserName = arr[i].toObject()["Username"].toString();
        NickName = arr[i].toObject()["Nickname"].toString();
        Kuang* k = new Kuang(UserName, NickName);
        kuangstore.append(k);
        connect(k, &Kuang::KuangChosenChanged, this, &MainWindow::SendNewFriendInfo);
        searchlayout->addWidget(k);
    }
}
//void MainWindow::SendNewFriendInfo(Kuang *This){
//    int j,n1 = friendsession.size();
//    if(This->USERNAME == username){
//        QMessageBox::information(this,"提示","不能添加自己为好友");
//        return;
//    }
//    for(j=0;j < n1;j++){
//        if(friendsession[j]->USERNAME == This->USERNAME){
//            QMessageBox::information(this,"提示",friendsession[j]->USERNAME + "已经是您的好友了");
//            return;
//        }
//    }
//    if(This->IsChosen){
//        QMessageBox::information(this,"提示","请勿重复发送好友请求");
//    }
//    else {
//        QMessageBox::information(this,"提示","添加好友信息已发送");
//        This->IsChosen = true;
//        emit SendNewFriendInfo1(This);
//    }
//}
void MainWindow::SendNewFriendInfo(Kuang* This)
{
    int j, n1 = friendsession.size(), n2 = groupsession.size();
    if (This->USERNAME == username) {
        QMessageBox::information(this, "提示", "不能添加自己为好友");
        return;
    }
    for (j = 0; j < n1; j++) {
        if (friendsession[j]->USERNAME == This->USERNAME) {
            QMessageBox::information(this, "提示", friendsession[j]->USERNAME + "已经是您的好友了");
            return;
        }
    }
    for (j = 0; j < n2; j++) {
        if (groupsession[j]->USERNAME == This->USERNAME) {
            QMessageBox::information(this, "提示", "您已经在群聊" + groupsession[j]->USERNAME + "中了");
            return;
        }
    }
    if (This->IsChosen) {
        QMessageBox::information(this, "提示", "请勿重复发送好友请求");
    } else {
        QMessageBox::information(this, "提示", "请求已发送");
        This->IsChosen = true;
        emit SendNewFriendInfo1(This);
    }
}
void MainWindow::showreddot(Kuang* This)
{
    This->Showreddot();
}

void MainWindow::hidereddot(Kuang* This)
{
    This->Hidereddot();
}

//xyc
void MainWindow::setUpUi_xyc()
{
    //背景
    QPixmap * background_pixmap = new QPixmap(":/new/prefix2/Resources/background.png");
    QLabel * qlabel = new QLabel(this);
    qlabel->setScaledContents(true);
    qlabel->setPixmap(*background_pixmap);
    qlabel->setGeometry(55,52,1333,920);
    qlabel->lower();
    qlabel->show();

    //对主界面头像label进行圆形头像载入，头像是测试图片
    QPixmap pixmap(":/new/prefix1/1e7603ac0bbf7a393db26e353eaf29cf.jpeg"); //测试png
    ui->profilePhoto->setPixmap(pixmap);
    ui->MyLable_HomePage_UserImage_Right->setPixmap(pixmap);

    //设置pane transparent可以使tabwidget背景色完全消失

    //设置lineedit选中框消失
    ui->nickNameShow->setAttribute(Qt::WA_MacShowFocusRect, 0);
    ui->userNameShow->setAttribute(Qt::WA_MacShowFocusRect, 0);
    ui->Signature->setAttribute(Qt::WA_MacShowFocusRect, 0);

    ui->tabWidget->setCurrentIndex(2);

    //使用PushButton代替Tab，Tab已经在.ui中进行了隐藏
    QVector<QPushButton*> pushButtonList_Navigator = {
        ui->pushButton_HomePage, //主页
        ui->pushButton_ChatPage, //聊天页
        ui->pushButton_NewFriendPage, //添加好友页
        ui->pushButton_NewGroupPage //新建群组页
    };
    for (int i = 0; i < 4; i++) {
        connect(pushButtonList_Navigator[i], &QPushButton::clicked, this, [=]() {
            if (i != 2) {
                ui->tabWidget->setCurrentIndex(i);
                ui->AddFiriend_SmallPage_buildInPage->hide();
            } else if (i == 2) {
                ui->tabWidget->setCurrentIndex(1);
                ui->AddFiriend_SmallPage_buildInPage->show();
            }
            //根据当前第i个按键，打开第i个页面
            for (int j = 0; j < 4; j++) {
                if (j == i)
                    continue;
                pushButtonList_Navigator[j]->setChecked(false);
            }
        });
    }

    //使用PushButton完善页面退出机制
    connect(ui->pushButton_Quit, &QPushButton::clicked, [=]() { this->close(); });

    //无边框实现
    //拖拽信息工具控件
    struct FramelessWindowPrivate {
        FramelessWindowPrivate(QWidget* contentWidget)
            : contentWidget(contentWidget)
        {
        }

        QWidget* contentWidget;
        QPoint mousePressedPosition; // 鼠标按下时的坐标
        QPoint windowPositionAsDrag; // 鼠标按小时窗口左上角的坐标
    };
    setWindowFlags(Qt::FramelessWindowHint); // 去掉边框
    setAttribute(Qt::WA_TranslucentBackground); //设置背景透明



    ui->listWidget->setAttribute(Qt::WA_MacShowFocusRect,false);
}
//xyc
void MainWindow::showNewFriendPop(QString name)
{
    AddFriendRequestPop* pop_A = new AddFriendRequestPop(this, name);

    pop_A->setGeometry((this->width() - pop_A->width()) / 2, (this->height() - pop_A->height()) / 2, this->width(), this->height());
    pop_A->show();
}
//xyc

void MainWindow::Sessionorderchange(QJsonObject data)
{
    int ID = data["SessionID"].toInt(), i, n;
    n = friendsession.size();
    for (i = 0; i < n; i++) {
        if (friendsession[i]->SessionID == ID) {
            friendlayout->removeWidget(friendsession[i]);
            friendlayout->insertWidget(0, friendsession[i]);
            if (data["SenderName"].toString() != username) {
                showreddot(friendsession[i]);
            }
            return;
        }
    }
    n = groupsession.size();
    for (i = 0; i < n; i++) {
        if (groupsession[i]->SessionID == ID) {
            grouplayout->removeWidget(groupsession[i]);
            grouplayout->insertWidget(0, groupsession[i]);
            if (data["SenderName"].toString() != username) {
                showreddot(groupsession[i]);
            }
            return;
        }
    }
}

void MainWindow::FriendSessionAdd(QJsonObject data)
{
    QLayoutItem* lastItem = friendlayout->itemAt(friendlayout->count() - 1); // 头像弹簧
    friendlayout->removeItem(lastItem);
    Kuang* k = new Kuang(username, data, this);
    friendlayout->addWidget(k);
    friendsession.append(k);
    connect(k, &Kuang::KuangChosenChanged, this, &MainWindow::clearlistview);
    Kuang* t = new Kuang(username, data, this);
    newgrouplayout->addWidget(t);
    kuanggroupstore.append(t);
    connect(t, &Kuang::KuangChosenChanged, this, &MainWindow::groupchanged);
    friendlayout->addStretch();
}

void MainWindow::groupchanged(Kuang* This)
{
    Kuang::KuangChosenNow = nullptr;
    if (This->IsChosen == true) {
        newgroupviewlayout->removeWidget(This);
        newgrouplayout->addWidget(This);
        This->IsChosen = false;

    } else {
        newgrouplayout->removeWidget(This);
        newgroupviewlayout->addWidget(This);
        This->IsChosen = true;
    }
}

void MainWindow::GroupSessionAdd(QJsonObject data)
{
    QLayoutItem* lastItem = grouplayout->itemAt(grouplayout->count() - 1); // 头像弹簧
    grouplayout->removeItem(lastItem);
    Kuang* k = new Kuang(data, this);
    grouplayout->addWidget(k);
    groupsession.append(k);
    connect(k, &Kuang::KuangChosenChanged, this, &MainWindow::clearlistview);
    grouplayout->addStretch();
}

void MainWindow::clearlistview(Kuang* This)
{
    ui->listWidget->clear();
    if (Kuang::IsShow) {
        Kuang::IsShow = false;
        ui->widget_2->show();
        //        lblBackground->hide();
        //        lblWelcome->hide();
    }
    ui->lblUsername->setText(This->USERNAME);
    hidereddot(This);
    emit updatelistview();
}

//fgm
void MainWindow::AddMessagetoListview(QJsonObject data)
{
    if (Kuang::KuangChosenNow && data["SessionID"].toInt() == Kuang::KuangChosenNow->SessionID) {
        hidereddot(Kuang::KuangChosenNow);
        QString time = QString::number(QDateTime::currentDateTime().toTime_t());
        dealMessageTime(time); //用于处理时间
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        QString msg = data["Body"].toObject()["Text"].toString();
        QString filedata;
        qDebug() << "recv=============" << msg;
        qDebug() << "recv ---------------" << data;
        //需要服务器返回有关用户头像的信息，建议在jsonobject中profile中添加
        QString touxiangurl = data["sendertouxiangURL"].toString();

        if(msg == "file/picture message"){
            QString fileURL;
            qDebug() <<" ====file/picture message====\n";
            fileURL = data["Body"].toObject()["url"].toString();
            if(data["SenderName"].toString() == username){
                Message *messageW = new Message(username,fileURL,ui->listWidget->parentWidget());
                dealMessage1(messageW, item, msg, fileURL, time, Message::userMe);
            }
            else{
                qDebug() << "Right:file/picture message====\n";
                Message *messageW = new Message(data["SenderName"].toString(),fileURL,ui->listWidget->parentWidget());
                dealMessage1(messageW, item, msg, fileURL, time, Message::userOther);
            }
        }else if (msg == "file message") {
            QMessageBox::information(this, "提示", "你收到一个文件");
            QJsonObject filedata1 = data["Body"].toObject();
            qDebug() << "11111111111111" << filedata1;
            QJsonObject filedata2 = filedata1["Picture/file"].toObject();
            qDebug() << "00000000000" << filedata2;
            QString fileName1 = filedata2["fileName"].toString();
            qDebug() << fileName1;
            QString content = filedata2["content"].toString();
            qDebug() << content;
            QByteArray arr = content.toUtf8();

            //生成文件
            file.setFileName(fileName1);
            bool isOk = file.open(QIODevice::WriteOnly);
            if (isOk == false) {
                qDebug() << "文件接收失败";
            } else {
                QMessageBox::information(this, "提示", "文件已全部接收");
                qint64 len = file.write(arr);
                file.close();
                qDebug() << "len === " << len;
            }
            if (data["SenderName"].toString() == username) {
                Message* messageW = new Message(username, ui->listWidget->parentWidget());
                dealMessage(messageW, item, msg, time, Message::userMe, touxiangurl);
            } else {
                Message* messageW = new Message(data["SenderName"].toString(), ui->listWidget->parentWidget());
                dealMessage(messageW, item, msg, time, Message::userOther, touxiangurl);
            }
        } else {
            if (data["SenderName"].toString() == username) {
                Message* messageW = new Message(username, ui->listWidget->parentWidget());
                dealMessage(messageW, item, msg, time, Message::userMe, touxiangurl);
            } else {
                Message* messageW = new Message(data["SenderName"].toString(), ui->listWidget->parentWidget());
                dealMessage(messageW, item, msg, time, Message::userOther, touxiangurl);
            }
        }
    }
}

//void MainWindow::on_sendgroup_clicked()
//{
//    QString msg = ui->lineEdit->text();
//    if (msg.size() == 0) {
//        QMessageBox::information(this, "提示", "请为您的群聊设置一个昵称");
//        return;
//    }
//    QJsonArray temp;
//    int i, n = kuanggroupstore.size();
//    for (i = 0; i < n; i++) {
//        if (kuanggroupstore[i]->IsChosen) {
//            temp.append(QJsonObject({ { "username", kuanggroupstore[i]->USERNAME } }));
//        }
//    }
//    temp.append(QJsonObject({ { "username", username } }));
//    QJsonObject data = { { "MsgType", "NewGroup" }, { "SessionName", msg } };
//    data["Members"] = temp;
//    emit sendnewgroup(data);

//    //清空群聊名称
//    ui->lineEdit->setText("");
//    //清空已选中好友
//    //    QLayoutItem * child = newgroupviewlayout->takeAt(0);
//    //    QLayoutItem * temp1 = child;
//    //    while(child != 0){
//    //        newgrouplayout->addItem(temp1);
//    //        child->widget()->setParent(NULL);
//    //        delete child;
//    //        child = newgroupviewlayout->takeAt(0);
//    //        temp1 = child;
//    //    }
//}

//void MainWindow::on_sendfile_cliked(){
//    qDebug() << "plz choose file";
//    QString filename = QFileDialog::getOpenFileName(this, tr("图片选择对话框"), "F:", tr("*png *jpg;"));
//    QImage image(filename);
//    QByteArray imgBy;
//    QBuffer imgBuf(&imgBy);
//    image.save(&imgBuf, "png");
//    emit charMsg(username, QString::fromLocal8Bit(imgBy.toBase64()));
//    //QString str = QString(QDateTime::currentDateTime().toString("hh:mm"));
//    qDebug() << "图片已发送";
//}

void MainWindow::on_sendfile_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("图片选择对话框"), "F:", tr("*png *jpg;"));
    if (!filename.isEmpty()) {
        QImage image(filename);
        QByteArray imgBy;
        QBuffer imgBuf(&imgBy);
        bool flag = image.save(&imgBuf, "png");
        if (flag) {
            emit charMsg(username, QString::fromLocal8Bit(imgBy.toBase64()));
        }
    }
}

void MainWindow::on_huantou_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("图片选择对话框"), "F:", tr("*png *jpg;"));
    qDebug() << filename;
    if (!filename.isEmpty()) {
        QImage image(filename);
        QPixmap pixmap = QPixmap::fromImage(image);
        QPixmap fitpixmap = pixmap.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        ui->profilePhoto->setScaledContents(true);
        ui->profilePhoto->setPixmap(fitpixmap);
        ui->MyLable_HomePage_UserImage_Right->setPixmap(fitpixmap);
    }
    emit touXiang(this->username, filename);
}

//fgm
void MainWindow::on_filebutton_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(this, tr("文件选择对话框"), "F:", tr("*txt;"));
    if (!filepath.isEmpty()) {
        qDebug() << "filepath" << filepath;
        QFileInfo info(filepath);
        fileName = info.fileName();
        fileSize = info.size();
        sendSize = 0;
        file.setFileName(filepath);
        bool isOk = file.open(QIODevice::ReadOnly);
        if (isOk == false) {
            qDebug() << "文件未成功打开";
        }
        qDebug() << isOk;
    } else {
        qDebug() << "文件打开失败";
    }
    QJsonObject filedata;
    filedata["MsgType"] = "filemsg";
    filedata["fileName"] = fileName;
    filedata["fileSize"] = fileSize;
    //读取文件内容
    char buf[4 * 1024] = { 0 };
    file.read(buf, sizeof(buf));
    filedata["content"] = buf;
    //QString str1 = QString(QJsonDocument(filedata).toJson());
    //qDebug() << "### send 11111" << str1;
    QMessageBox::information(this, "提示", "文件发送成功");
    emit sendfile1(username, filedata);
}

void MainWindow::on_selectfile_clicked()
{
}

void MainWindow::on_changeSignature_clicked()
{
    QJsonObject json;
    json["MsgType"] = "SignatureChange";
    json["username"] = ui->userNameShow->text();
    json["Signature"] = ui->Signature->text();
    QMessageBox::information(this, "提示", "更改成功");
    emit signatureChange(json);
}

//xyc
#pragma region 无边框函数实现 {

//无边框部分，响应拖拽，override

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) // 左键按下
    {
        m_bMove = true; // 移动标志位
        reltvPos = event->pos(); // 按下瞬间获取相对窗口坐标
    }
    return QWidget::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (m_bMove)
        move(event->globalPos() - reltvPos);
    return QWidget::mouseMoveEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    m_bMove = false; // 松开后要置为false
    return QWidget::mouseReleaseEvent(event);
}

#pragma endregion }
//xyc

void MainWindow::on_shake_clicked()
{
    QPropertyAnimation* pAnimation = new QPropertyAnimation(this, "pos");
    pAnimation->setDuration(500);
    pAnimation->setLoopCount(2);
    pAnimation->setKeyValueAt(0.1, QPoint(geometry().x() + 6, geometry().y() + 6));
    pAnimation->setKeyValueAt(0.2, QPoint(geometry().x() - 6, geometry().y() + 6));
    pAnimation->setKeyValueAt(0.3, QPoint(geometry().x() + 6, geometry().y() - 6));
    pAnimation->setKeyValueAt(0.4, QPoint(geometry().x() - 6, geometry().y() - 6));
    pAnimation->setKeyValueAt(0.5, QPoint(geometry().x() + 6, geometry().y() + 6));
    pAnimation->setKeyValueAt(0.6, QPoint(geometry().x() - 6, geometry().y() + 6));
    pAnimation->setKeyValueAt(0.7, QPoint(geometry().x() + 6, geometry().y() - 6));
    pAnimation->setKeyValueAt(0.8, QPoint(geometry().x() - 6, geometry().y() + 6));
    pAnimation->setKeyValueAt(0.9, QPoint(geometry().x() + 6, geometry().y() + 12));
    pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

//dyx
void MainWindow::on_toolButton_emoji_clicked()
{
    emoji *emojiWindow = new emoji(username);
    emojiWindow->setGeometry(620,851,500,360);
    emojiWindow->show();
    connect(emojiWindow, &emoji::charMsg, this, &MainWindow::charMsg);
}

void MainWindow::on_sendgroup_clicked()
{
    QString msg = ui->lineEdit->text();
    if (msg.size() == 0) {
        QMessageBox::information(this, "提示", "请为您的群聊设置一个昵称");
        return;
    }
    QJsonArray temp;
    int i, n = kuanggroupstore.size();
    for (i = 0; i < n; i++) {
        if (kuanggroupstore[i]->IsChosen) {
            temp.append(QJsonObject({ { "username", kuanggroupstore[i]->USERNAME } }));
        }
    }
    temp.append(QJsonObject({ { "username", username } }));
    QJsonObject data = { { "MsgType", "NewGroup" }, { "SessionName", msg } };
    data["Members"] = temp;
    emit sendnewgroup(data);

    //清空群聊名称
    ui->lineEdit->setText("");
    //清空已选中好友
    //    QLayoutItem * child = newgroupviewlayout->takeAt(0);
    //    QLayoutItem * temp1 = child;
    //    while(child != 0){
    //        newgrouplayout->addItem(temp1);
    //        child->widget()->setParent(NULL);
    //        delete child;
    //        child = newgroupviewlayout->takeAt(0);
    //        temp1 = child;
    //    }
}
