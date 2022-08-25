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
    explicit UserRegister(QWidget *parent = nullptr);
    ~UserRegister();
    void registconfirm(QJsonObject data);

signals:
    void registfinished();
    void sendregistdata(QJsonObject data);
private slots:

    void on_lERgPassword1_returnPressed();

    void on_pushButton_Back_clicked();

private:
    Ui::UserRegister *ui;

    //xyc
    //无边框部分，坐标成员变量
    QPoint reltvPos; // 相对坐标
    bool m_bMove; // 是否移动
    //xyc

protected:

    //xyc
    //无边框部分，override拖拽事件
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    //xyc

};

#endif // USERREGISTER_H
