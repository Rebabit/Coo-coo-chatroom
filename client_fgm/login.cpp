#include "login.h"
#include "ui_login.h"



Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    //点击注册跳转到注册页面
    connect(ui->btnRegister, &QPushButton::clicked, this, [=](){
        this->hide();
        emit registerButtonClicked();
    });

    //点击登录按钮发送登录信息
    connect(ui->btnLogin, &QPushButton::clicked, this, [=](){
        QJsonObject login = {{"MsgType","LogIn"},{"Username",ui->lineEditUserName->text()},{"Password",ui->lineEditPassword->text()}};
        emit sendlogindata(login);
    });


}

void Login::loginconfirm(QJsonObject data){
    if(data["IsLegal"].toBool() == false){
        QMessageBox::critical(this, "Error!", "用户名或密码错误");
    }else{
        this->hide();
        emit createMainWindow(data);
    }
}



Login::~Login()
{
    delete ui;
}
