#include "msgsession.h"
#include "ui_msgsession.h"
#include<QJsonArray>
#include<QMouseEvent>
#include <QDebug>
#include "clientdatacenter.h"
#include<QPainter>


QPixmap MsgSession::PaintHead(const QChar &name)
{
    QSize size(50,50); //指定图片大小;
    QImage image(size,QImage::Format_ARGB32);
    //以ARGB32格式构造一个QImage
    //image.fill(qRgba(0,0,0,100));//填充图片背景,0-255为透明度
    image.fill(qRgba(52,73,94,255));
    QPainter painter(&image); //为这个QImage构造一个QPainter
    painter.setCompositionMode(QPainter::CompositionMode_SourceOut);
    //设置画刷的组合模式CompositionMode_SourceOut这个模式为目标图像在上。
    //改变组合模式和上面的填充方式可以画出透明的图片。

    //改变画笔和字体
    QPen pen = painter.pen();
    pen.setColor(QColor(255,255,255,255));
    QFont font = painter.font();
    font.setFamily("Microsoft YaHei");
    font.setBold(true);//加粗
    font.setPixelSize(25);//改变字体大小

    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(image.rect(),Qt::AlignCenter, name);
    //将Hello写在Image的中心ui
    return QPixmap::fromImage(image);
}


MsgSession::MsgSession(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MsgSession)
{
    ui->setupUi(this);
}

//私聊头像及消息
MsgSession::MsgSession(const QString &username, QJsonObject data, QWidget *parent):QWidget(parent), ui(new Ui::MsgSession){
    ui->setupUi(this);
    QJsonArray temp = data["Members"].toArray();
    SessionID = data["SessionId"].toInt();
    if(temp[0].toObject()["Username"].toString() == username){
        ui->name->setText(temp[1].toObject()["Username"].toString());
    }else{
        ui->name->setText(temp[0].toObject()["Username"].toString());
    }
    //ui->profile
}

//群聊消息框
MsgSession::MsgSession(QJsonObject data, QWidget *parent):QWidget(parent),ui(new Ui::MsgSession){
    ui->setupUi(this);
    SessionID = data["SessionId"].toInt();
    ui->name->setText(data["profile"].toObject()["SessionName"].toString());
}
MsgSession* MsgSession::MsgSessionChosenNow = nullptr;


//鼠标左键切换到对应的聊天框
void MsgSession::mousePressEvent(QMouseEvent *ev){
    qDebug()<< "Mouse Pressed" << ClientDataCenter::Singleton().getSession(SessionID).getLatestMessageID();
    if(ev->button() == Qt::LeftButton){
        if(!MsgSessionChosenNow || MsgSessionChosenNow != this){
            MsgSessionChosenNow = this;
            emit msgSessionChanged(this);
        }
    }
}
void MsgSession::Showreddot(){
    ui->lblNewMessage->show();
}

void MsgSession::Hidereddot(){
    ui->lblNewMessage->hide();
}
MsgSession::MsgSession(QString UserName,QString NicName,QWidget *parent):
    QWidget(parent),
    ui(new Ui::MsgSession){
    ui->setupUi(this);
    ui->lblNewMessage->hide();
    SessionID = 0;
    IsChosen = false;
    USERNAME = UserName;
    ui->name->setText(UserName);
    ui->profile->setPixmap(PaintHead(USERNAME[0]));
}

bool MsgSession::IsShow = true;


MsgSession::~MsgSession()
{
    delete ui;
}
