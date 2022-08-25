#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "sever.h"

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
//    void on_pushButton_3_clicked() {}

    void printLink(QString);             //收到由server发来窗口的信号，并且打印到文本框
    void printMsg(QList<QJsonObject>, int);

    void on_btnSend_clicked();

    void on_btnRadio_clicked();

    void setChannel(int);

    void on_btnSetServer_clicked();

    void setOffline(int);
    void load_Users();

    void getMessageTargetandContent(int, QJsonObject);


signals:
    void pushIP(QString);

private:
    Ui::Widget *ui;

    Sever sever;        //实例化一个sever对象
};
#endif // WIDGET_H
