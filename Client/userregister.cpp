#include "userregister.h"
#include "ui_userregister.h"
#include<QString>
#include<QDebug>
#include<QJsonObject>
#include<QJsonArray>
#include<qmessagebox.h>
#include<QMouseEvent>


UserRegister::UserRegister(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserRegister)
{
    ui->setupUi(this);

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

    connect(ui->btnRegister,&QPushButton::clicked,[=](){
        bool islegal = true;
        QJsonObject registration_info;
        QString s = ui->lERgUserName->text(), s1;
        if(s.size()==0){
            QMessageBox::critical(this,"Error!","用户名不能为空");
            islegal = false;
        }
        s = ui->lERgPassword1->text();
        s1 = ui->lERgPassword2->text();
        if((s.size()==0 || s1.size()==0) && islegal){
            QMessageBox::critical(this,"Error!","密码不能为空");
            islegal = false;
        }
        if(ui->lERgNickName->text().size()==0 && islegal){
            QMessageBox::information(this,"提示","请设置您的昵称");
            islegal = false;
        }
        if(s != s1 && islegal){
            QMessageBox::critical(this,"Error!","两次输入的密码不一致");
            islegal = false;
        }
        if(islegal){
            //发送注册信息
            registration_info.insert("Username",ui->lERgUserName->text());
            registration_info.insert("Nickname",ui->lERgNickName->text());
            registration_info.insert("Password",ui->lERgPassword1->text());
            registration_info.insert("MsgType","Regist");
            sendregistdata(registration_info);
        }
    });
}

void UserRegister::registconfirm(QJsonObject data){
    bool legal = data["IsLegal"].toBool();
    if(legal){
        QMessageBox::information(this,"提示","注册成功");
        emit UserRegister::registfinished();
        this->close();
    }
    else{
        QMessageBox::critical(this,"Error!","用户名已存在");
    }
}

UserRegister::~UserRegister()
{
    delete ui;
}

void UserRegister::on_lERgPassword1_returnPressed() {
    emit ui->btnRegister->clicked();
}



//xyc 返回按钮
void UserRegister::on_pushButton_Back_clicked()
{
    emit registfinished(); //返回，效果等同于注册完成
}
//无边框部分，响应拖拽，override

void UserRegister::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) // 左键按下
    {
        m_bMove = true; // 移动标志位
        reltvPos = event->pos(); // 按下瞬间获取相对窗口坐标
    }
    return QWidget::mousePressEvent(event);
}

void UserRegister::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bMove) move(event->globalPos() - reltvPos);
    return QWidget::mouseMoveEvent(event);
}

void UserRegister::mouseReleaseEvent(QMouseEvent *event)
{
    m_bMove = false; // 松开后要置为false
    return QWidget::mouseReleaseEvent(event);
}
//xyc

