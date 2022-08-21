#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QJsonObject>
#include <QMainWindow>
#include <QVBoxLayout>
#include "msgsession.h"
#include <QPushButton>
#include <QDebug>
#include <QString>
#include <QJsonArray>
#include <QListWidget>
#include <QLabel>
#include "message.h"

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
    QVector<MsgSession*>kuangstore,kuanggroupstore,friendsession,groupsession;
    //处理信息
    void dealMessage(Message *messageW, QListWidgetItem *item, QString text, QString time, Message::UserType type);
    //处理搜索框好友显示
    void dealNewFriend(QString text);
    //处理时间
    void dealMessageTime(QString curMsgTime);
    void AddMessagetoListview(QJsonObject data);
    void clearlistview(MsgSession *This);
    void dealsearchinfo(QJsonObject data);
    void groupchanged(MsgSession *This);
    void Sessionorderchange(QJsonObject data);
    void SendNewFriendInfo(MsgSession *This);
    void showreddot(MsgSession *This);
    void hidereddot(MsgSession *This);

signals:
    void SendNewFriendInfo1(MsgSession *This);
    void updatelistview();
    void SendMessageToServer(const QString &sendername,const QString &text);
    bool eventFilter(QObject *watched, QEvent *event);
    void SendSearchInfo(QString msg);
    void sendnewgroup(QJsonObject data);


private slots:
    void on_btnSend_clicked();
    void on_sendgroup_clicked();

protected:
    //重生事件 调整聊天框大小
    void resizeEvent(QResizeEvent *event);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
