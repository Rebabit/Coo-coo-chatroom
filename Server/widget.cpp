#include "widget.h"
#include "ui_widget.h"
#include "operations.h"
#include<QJsonObject>
#include<QJsonDocument>
#include "databaseoperation.h"

//发送和广播按钮没用

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //将server的信息打印在widget中
    connect(&server,&Server::linkMsg,this,&Widget::printLink);//打印字符串
    connect(&server, &Server::sendMsg,this, &Widget::printMsg);//打印信息
    connect(&server, &Server::offLine,this,&Widget::setOffline);//设置下线

    //每个新客户端的socket首次连接时，将handle可视化在widget中
    connect(&server,&Server::sendChannel,this,&Widget::setChannel);

    //当widget发出pushIP信号，server开启listen模式
    connect(this,&Widget::pushIP,&server,&Server::setIP);
    //数据库连接成功，展示所有服务器开启前的用户（有空可以改）
    connect(&(DatabaseOperation::Singleton()), &DatabaseOperation::signal_DB_ready, this, &Widget::load_Users);
    //接受新信息
    connect(&(Operations::Singleton ()),&Operations::newMessage,this, &Widget::getMessageTargetandContent);
}

Widget::~Widget()
{
    DatabaseOperation::Singleton().closeDB();
    delete ui;
}

//打印字符串
void Widget::printLink(QString str)
{
    ui->listWidget_2->addItem (str);
    ui->listWidget_2->setCurrentRow (ui->listWidget_2->count ()-1);
}

//将消息打包传输给客户端
void Widget::printMsg(QList<QJsonObject> list, int handle)
{
    qDebug() << "in printMsg!";
    // Package包括MsgType和一串的QJsonArray
    QJsonObject Package = {{ "MsgType", "JsonArray" }};
    QJsonArray array;
    for (int i = 0; i < list.size(); i++)
    {
        array.append(list[i]);
    }
    Package["MsgList"] = array;
    //将QJsonObject转为供传输的QByteArray，并发送给客户端
    QJsonDocument send1 = QJsonDocument(Package);
    QByteArray send2 = send1.toJson();
    server.clientMap[handle]->write(send2);
}

//将handle可视化在widget中,加入combobox待选
void Widget::setChannel(int channel)
{
    qDebug()<<"setchannel";
    if(ui->comboBox->findText(QString::number (channel)) == -1)
        qDebug()<<QString::number (channel);
        ui->comboBox->addItem (QString::number (channel));
}

//设置服务器IP，server开启listen模式
void Widget::on_btnSetServer_clicked()
{
    emit pushIP (ui->textEdit->toPlainText ());
}

//实现用户下线
void Widget::setOffline(int handle)
{
    //UI界面显示下线
    int index = ui->comboBox->findText(QString::number (handle));
    if(index != -1 )
        ui->comboBox->setItemText (index, "disconnected.");
    QString str = server.clientMap[handle]->peerAddress ().toString ()+"断开连接！";
    ui->listWidget_2->addItem (str);
    ui->listWidget_2->setCurrentRow (ui->listWidget_2->count ()-1);

    //server中移除handle和usertohandle的部分
    server.clientMap.remove(handle);
    for(QMap<QString, QList<int>>::iterator it1 = server.userToHandle.begin();it1!=server.userToHandle.end();it1++)
    {
        it1.value().removeAll(handle);//一个handle对应一个账号，把该handle从用户名对应的list中删除
    }
    //qDebug() << server.userToHandle;
}

//显示所有在服务器开启前已注册的用户
void Widget::load_Users() {
    auto& db = DatabaseOperation::Singleton();
    auto list = db.findAllUsers();
    for (int i = 0; i < list.size(); i++) {
        this->ui->userWidgets->addItem(list[i]->getNickname() + " @" + list[i]->getUsername());
    }
}

void Widget::getMessageTargetandContent(int sessionID, QJsonObject msg)
{
    auto& db = DatabaseOperation::Singleton();
    QList<QString> sessionList = db.queryMembersBySession (sessionID);
    QJsonDocument send1 = QJsonDocument(msg);
    QByteArray send2 = send1.toJson ();
    //所有会话中的成员
    for (int i = 0; i < sessionList.size(); i++)
    {
        //所有终端
        for (QMap <QString, QList<int>>::iterator it1 = server.userToHandle.begin();it1!=server.userToHandle.end ();it1++)
        {
            if (it1.key() == sessionList[i])
            {
                qDebug()<< "找到相同user名";
                if(it1.value ().size ()==0)
                    break;//当前成员没有终端登陆
                for(int j=0;j<it1.value ().size ();j++) {
                    server.clientMap[it1.value ()[j]]->write (send2.data ());
                }
            }
            break;
        }
    }
}
