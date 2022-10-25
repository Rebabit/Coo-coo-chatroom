#ifndef SERVERMAINWINDOW_H
#define SERVERMAINWINDOW_H

#include <QMainWindow>
#include "tcp_server.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ServerMainWindow; }
QT_END_NAMESPACE

class ServerMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    ServerMainWindow(QWidget *parent = nullptr);
    ~ServerMainWindow();

private:
    Ui::ServerMainWindow *ui;
    TCP_Server & server;

private slots:
    void slotUpdateIPList();
    void on_toggleServer_clicked();
    void slot_handleServerStarted();
    void slot_handleServerStopped();
    void printMes(QString, int);             //收到由server发来窗口的信号，并且打印到文本框
    void on_btnSend_clicked();
};
#endif // SERVERMAINWINDOW_H
