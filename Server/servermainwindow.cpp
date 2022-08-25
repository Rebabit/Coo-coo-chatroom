#include "servermainwindow.h"
#include "ui_servermainwindow.h"
#include <QTcpServer>
ServerMainWindow::ServerMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ServerMainWindow)
    , server(TCP_Server::Singleton())
{
    ui->setupUi(this);

    // IP 地址更新到 comboBox
    connect(&server, &TCP_Server::signal_IP_ListUpdate, this, &ServerMainWindow::slotUpdateIPList);

    // port 只能输入数字
    QRegExp regx_number("[0-9]+$");
    ui->portInput->setValidator(new QRegExpValidator(regx_number, ui->portInput));

    // 处理服务器开启、停止的 ui事件
    connect(&server, &TCP_Server::signal_ServerStart, this, &ServerMainWindow::slot_handleServerStarted);
    connect(&server, &TCP_Server::signal_ServerStop, this, &ServerMainWindow::slot_handleServerStopped);

    connect(&server, &TCP_Server::sendMsg,this, &ServerMainWindow::printMes);
}

ServerMainWindow::~ServerMainWindow()
{
    delete ui;
    delete &TCP_Server::Singleton();
}

// 更新ip地址下拉菜单中的内容
void ServerMainWindow::slotUpdateIPList()
{
    if (server.getIPList().count() == 0) return;
    ui->ipAddressBox->clear();
    ui->ipAddressBox->addItems(server.getIPList());
}

// 开启/关闭服务器
void ServerMainWindow::on_toggleServer_clicked()
{
    bool server_state = server.isRunning();
    if (server_state) {
        server.stopServer();
    }
    else {
       uint port_number = ui->portInput->text().toUInt();
       server.runServer(ui->ipAddressBox->currentText(), port_number);

    }
}

// 服务器停止后，激活连接服务器的ui
void ServerMainWindow::slot_handleServerStopped()
{
    ui->toggleServer->setText("Run Server");
    ui->ipAddressBox->setEnabled(true);
    ui->portInput->setEnabled(true);
}

void ServerMainWindow::printMes(QString str, int type)
{
    if(type==1)
        ui->teReceive->setText (str);
    else ui->listWidget->addItem (str);
}

// 服务器开启后，禁用连接服务器相关的ui
void ServerMainWindow::slot_handleServerStarted()
{
    ui->toggleServer->setText("Stop");
    ui->ipAddressBox->setEnabled(false);
    ui->portInput->setEnabled(false);
}

void ServerMainWindow::on_btnSend_clicked()
{
//    server.socket->write(ui->teSend->toPlainText ().toUtf8 ());
}
