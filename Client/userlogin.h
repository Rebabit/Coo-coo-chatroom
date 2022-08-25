#ifndef USERLOGIN_H
#define USERLOGIN_H

#include <QWidget>
#include<QJsonObject>
#include<mainwindow.h>
#include<QTcpSocket>
#include<userregister.h>

QT_BEGIN_NAMESPACE
namespace Ui { class UserLogin; }
QT_END_NAMESPACE

class UserLogin : public QWidget
{
    Q_OBJECT

public:
    MainWindow *main = nullptr;
    UserLogin(QWidget *parent = nullptr);
    ~UserLogin();
    void loginconfirm(QJsonObject data);
signals:
    void registerButtonClicked();
    void sendlogindata(QJsonObject data);
    void createMainWindow(QJsonObject data);

protected:
    //xyc
    //无边框部分，override拖拽事件
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    //xyc
private slots:

    void on_lineEditPassword_returnPressed();

private:
    Ui::UserLogin *ui;

    //xyc
    //无边框部分，坐标成员变量
    QPoint reltvPos; // 相对坐标
    bool m_bMove; // 是否移动
    //xyc
};
#endif // USERLOGIN_H
