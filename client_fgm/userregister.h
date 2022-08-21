#ifndef USERREGISTER_H
#define USERREGISTER_H

#include <QWidget>
#include<QJsonArray>
#include<QJsonObject>
namespace Ui {
class UserRegister;
}

class UserRegister : public QWidget
{
    Q_OBJECT

public:
    explicit UserRegister(QWidget *parent = 0);
    ~UserRegister();
    void registconfirm(QJsonObject data);       //接收并处理服务器返回的注册信息


signals:
    void registfinished();                      //注册完成信号
    void sendregistdata(QJsonObject data);      //发送注册数据信号

private slots:
    void on_lERgPassword1_returnPressed();      //返回登录界面信号

private:
    Ui::UserRegister *ui;
};

#endif // USERREGISTER_H
