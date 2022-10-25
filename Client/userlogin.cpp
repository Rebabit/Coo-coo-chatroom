#include "userlogin.h"
#include "ui_userlogin.h"
#include<userregister.h>
#include<QJsonObject>
#include<qmessagebox.h>
#include<QDebug>
#include"mainwindow.h"
#include<QMessageBox>
#include<QHostAddress>
#include<QJsonDocument>
#include<clientmain.h>



UserLogin::UserLogin(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserLogin)
{
    ui->setupUi(this);

    //点击注册跳转到注册页面
    connect(ui->btnRegister,&QPushButton::clicked,[=](){
        this->hide();
        emit registerButtonClicked();
    });

    //xyc
    //拖拽信息工具控件
    struct FramelessWindowPrivate {
        FramelessWindowPrivate(QWidget *contentWidget) : contentWidget(contentWidget) {}

        QWidget *contentWidget;
        QPoint mousePressedPosition; // 鼠标按下时的坐标
        QPoint windowPositionAsDrag; // 鼠标按小时窗口左上角的坐标
    };
    setWindowFlags(Qt::FramelessWindowHint); // 去掉边框
    setAttribute(Qt::WA_TranslucentBackground); //设置背景透明
    //xyc


//#define UIDEBUG
#ifndef UIDEBUG
    //点击登录发送登录信息
    connect(ui->btnLogIn,&QPushButton::clicked,[=](){
        QJsonObject login = { {"MsgType","LogIn"},{"Username",ui->lineEditUserName->text()},{"Password",ui->lineEditPassword->text()}};
        emit sendlogindata(login);
    });
#else
    //调试入口
    connect(ui->btnLogIn, &QPushButton::clicked,[=](){
        MainWindow *main = new MainWindow();
        main->show();
    });
#endif

    // main = new MainWindow(this);
}

void UserLogin::loginconfirm(QJsonObject data){
    if(data["IsLegal"].toBool()==false){
        QMessageBox::critical(this,"Error!","用户名或密码有误");
    }
    else{
        this->hide();
        emit createMainWindow(data);
    }
}

UserLogin::~UserLogin()
{
    delete ui;
}


void UserLogin::on_lineEditPassword_returnPressed()
{
    emit ui->btnLogIn->clicked();
}


//xyc
//无边框部分，响应拖拽，override

void UserLogin::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) // 左键按下
    {
        m_bMove = true; // 移动标志位
        reltvPos = event->pos(); // 按下瞬间获取相对窗口坐标
    }
    return QWidget::mousePressEvent(event);
}

void UserLogin::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bMove) move(event->globalPos() - reltvPos);
    return QWidget::mouseMoveEvent(event);
}

void UserLogin::mouseReleaseEvent(QMouseEvent *event)
{
    m_bMove = false; // 松开后要置为false
    return QWidget::mouseReleaseEvent(event);
}
//xyc
