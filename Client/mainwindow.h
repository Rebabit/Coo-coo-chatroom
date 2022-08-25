#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QJsonObject>
#include <QMainWindow>
#include<QVBoxLayout>
#include<kuang.h>
#include<QPushButton>
#include<QDebug>
#include<QString>
#include<QJsonArray>
#include <QListWidget>
#include <QLabel>
#include "message.h"
#include <QTcpSocket>
#include <QTimer>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    MainWindow(QJsonObject data,QWidget *parent = nullptr);
    ~MainWindow();
    void setup(QJsonObject data);
    void FriendSessionAdd(QJsonObject data);
    void GroupSessionAdd(QJsonObject data);
    QVBoxLayout *friendlayout;
    QVBoxLayout *grouplayout;
    QLabel *lblBackground;
    QLabel *lblWelcome;
    QVBoxLayout *searchlayout;
    QVBoxLayout *newgrouplayout;
    QVBoxLayout *newgroupviewlayout;
    QString username;
    QVector<Kuang*>kuangstore,kuanggroupstore,friendsession,groupsession;

    QString fileName;  //文件名
    qint64 fileSize;   //文件大小
    qint64 sendSize;   //已发送的文件大小
    QTcpSocket* tcpSocket;
    QFile file;

    //处理信息
    void dealMessage(Message *messageW, QListWidgetItem *item, QString text, QString time, Message::UserType type, QString touxiangurl);
    void dealMessage1(Message *messageW, QListWidgetItem *item, QString text, QString file, QString time, Message::UserType type);
    //处理搜索框好友显示
    void dealNewFriend(QString text);
    //处理时间
    void dealMessageTime(QString curMsgTime);
    void AddMessagetoListview(QJsonObject data);
    void clearlistview(Kuang *This);
    void dealsearchinfo(QJsonObject data);
    void groupchanged(Kuang *This);
    void Sessionorderchange(QJsonObject data);
    void SendNewFriendInfo(Kuang *This);
    void showreddot(Kuang *This);
    void hidereddot(Kuang *This);
    //void on_sendfile_cliked();

    //xyc
    void setUpUi_xyc();
    //xyc

    //xyc显示弹窗
    void showNewFriendPop(QString name);

signals:
    void SendNewFriendInfo1(Kuang *This);
    void updatelistview();
    void SendMessageToServer(const QString &sendername,const QString &text);
    bool eventFilter(QObject *watched, QEvent *event);
    void SendSearchInfo(QString msg);
    void sendnewgroup(QJsonObject data);
    void charMsg(QString username_, QString data);
    void touXiang(QString username1, QString touxiangurl);
    void signatureChange(QJsonObject);
    void sendfile(QString);
    void sendfile1(QString, QJsonObject);

private slots:
    void on_btnSend_clicked();
//    void on_sendgroup_clicked();
    void on_sendfile_clicked();

    void on_huantou_clicked();

    void on_filebutton_clicked();

    void on_changeSignature_clicked();

    void on_selectfile_clicked();

    void on_shake_clicked();

    void on_toolButton_emoji_clicked();

    void on_sendgroup_clicked();

protected:
    //重生事件 调整聊天框大小
    void resizeEvent(QResizeEvent *event);


    //xyc
    //无边框部分，override拖拽事件
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    //xyc


private:
    Ui::MainWindow *ui;

    //xyc
    //无边框部分，坐标成员变量
    QPoint reltvPos; // 相对坐标
    bool m_bMove; // 是否移动
    //xyc

};

#endif // MAINWINDOW_H
