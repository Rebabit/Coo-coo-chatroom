#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "server.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:

    void printLink(QString);             //将QString打印出来
    void printMsg(QList<QJsonObject>, int);

    void setChannel(int);
    void setOffline(int);

    void on_btnSetServer_clicked();

    void load_Users();
    void getMessageTargetandContent(int, QJsonObject);


signals:
    void pushIP(QString);

private:
    Ui::Widget *ui;
    Server server;        //实例化一个sever对象
};
#endif // WIDGET_H
