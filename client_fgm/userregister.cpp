#include "userregister.h"
#include "ui_userregister.h"
#include<QString>
#include<QDebug>
#include<QJsonObject>
#include<QJsonArray>
#include<qmessagebox.h>



UserRegister::UserRegister(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserRegister)
{
    ui->setupUi(this);
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
