#include "widget.h"
#include "ui_widget.h"
#include "operations.h"
#include<QJsonObject>
#include<QJsonDocument>
#include "databaseoperation.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //widget接受来自server的信息，并进行打印
    connect(&sever, &Sever::linkMsg,this, &Widget::printLink);
    connect(&sever, &Sever::sendMsg,this, &Widget::printMsg);
    connect(&sever, &Sever::offLine,this,&Widget::setOffline);

    //socket首次连接之后设置handle
    connect(&sever, &Sever::sendChannel,this,&Widget::setChannel);

    //widget发出setip信号，给server设置ip，开启listen
    connect(this,&Widget::pushIP,&sever, &Sever::setIP);

    connect(&(DatabaseOperation::Singleton()), &DatabaseOperation::signal_DB_ready, this, &Widget::load_Users);

    connect(&(Operations::Singleton ()),&Operations::newMessage,this, &Widget::getMessageTargetandContent);

}

Widget::~Widget()
{
    DatabaseOperation::Singleton().closeDB();
    delete ui;
}

void Widget::printLink(QString str)
{
    ui->listWidget_2->addItem (str);
    ui->listWidget_2->setCurrentRow (ui->listWidget_2->count ()-1);
}

void Widget::printMsg(QList<QJsonObject> list, int handle)
{
    //针对于regist、login、search请求仅回发单个socket
    qDebug() << "in printMsg!";
    QJsonObject Package = {{ "MsgType", "JsonArray" }};
    QJsonArray array;
    for(int i=0;i<list.size ();i++) {
        QJsonObject send = list[i];
        array.append(list[i]);
    }
    Package["MsgList"] = array;

    qDebug() << Package;
    QJsonDocument send1 = QJsonDocument(Package);
    QByteArray send2 = send1.toJson ();

    qDebug() << send2;

    sever.clientMap[handle]->write(send2);


}

void Widget::on_btnSend_clicked()
{
    qDebug() << "not implemented Widget::on_btnSend_clicked";
//    //发送功能实现，点对点
//    int channelName = ui->comboBox->currentText ().toUInt ();
//    QByteArray sendText = ui->teSend->toPlainText ().toUtf8 ();
//    //通过handle键取到值
//    sever.clientMap[channelName]->write (sendText);
}

void Widget::on_btnRadio_clicked()
{
    qDebug() << "not implemented Widget::on_btnRadio_clicked";
//    //广播功能实现，点对多点
//    QMap<int, QTcpSocket*>::iterator it;
//    for ( it = sever.clientMap.begin(); it != sever.clientMap.end(); ++it )
//    {
//        it.value ()->write (ui->teSend->toPlainText ().toUtf8 ());
//    }
}

void Widget::setChannel(int channel)
{
    //将handle加入combobox待选
    qDebug()<<"setchannel";
    if(ui->comboBox->findText(QString::number (channel)) == -1)
        qDebug()<<QString::number (channel);
        ui->comboBox->addItem (QString::number (channel));
}



void Widget::on_btnSetServer_clicked()
{

    emit pushIP (ui->textEdit->toPlainText ());

}

void Widget::setOffline(int handle)
{
    //下线功能，非常重要！！！
    int index = ui->comboBox->findText(QString::number (handle));
    if(index != -1 )
        ui->comboBox->setItemText (index, "disconnected.");
    QString str = sever.clientMap[handle]->peerAddress ().toString ()+"断开连接！";
    ui->listWidget_2->addItem (str);
    ui->listWidget_2->setCurrentRow (ui->listWidget_2->count ()-1);

    sever.clientMap.remove (handle);
    //删除usertohandle的表格
    for(QMap <QString, QList<int>>::iterator it1 = sever.userToHandle.begin();it1!=sever.userToHandle.end ();it1++)
    {
        it1.value ().removeAll (handle);

    }
    qDebug() << sever.userToHandle;

}
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
    for(int i=0;i<sessionList.size ();i++) {
        //所有成员的终端
        for(QMap <QString, QList<int>>::iterator it1 = sever.userToHandle.begin();it1!=sever.userToHandle.end ();it1++) {
            if(it1.key ()==sessionList[i]) {
                qDebug()<< "找到相同user名";
                if(it1.value ().size ()==0)
                    break;//当前成员没有终端登陆
                qDebug() << it1.key() << send2.data();
                for(int j=0;j<it1.value ().size ();j++) {
                    sever.clientMap[it1.value ()[j]]->write (send2.data ());
                    qDebug() << it1.value()[j];
                }
                break;
            }
        }
    }
}
