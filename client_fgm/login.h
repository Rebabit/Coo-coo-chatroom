#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QTcpSocket>
#include <QDebug>
#include <QMessageBox>
#include "mainwindow.h"


namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();
    void loginconfirm(QJsonObject data);
signals:
    void registerButtonClicked();
    void sendlogindata(QJsonObject data);
    void createMainWindow(QJsonObject data);


private:
    Ui::Login *ui;

};

#endif // LOGIN_H
