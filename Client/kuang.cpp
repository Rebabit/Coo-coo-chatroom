#include "kuang.h"
#include "ui_kuang.h"
#include<QJsonArray>
#include<QMouseEvent>
#include <QDebug>
#include "clientdatacenter.h"
#include<QPainter>

QPixmap Kuang::PaintHead(const QChar &name)
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

Kuang::Kuang(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Kuang)
{
    ui->setupUi(this);
}

// FRIEND
Kuang::Kuang(const QString &username,QJsonObject data,QWidget *parent):
    QWidget(parent),
    ui(new Ui::Kuang){
    ui->setupUi(this);
    ui->lblNewMessage->hide();
    QJsonArray temp = data["Members"].toArray();
    SessionID = data["SessionID"].toInt();
    qDebug() << "            " << data;
    //ClientDataCenter & dcenter = ClientDataCenter::Singleton();
    if(temp[0].toObject()["Username"].toString()==username){
        USERNAME = temp[1].toObject()["Username"].toString();
        ui->name->setText(USERNAME);
        //qDebug() << "=====1=====" <<USERNAME;
    }
    else {
        USERNAME = temp[0].toObject()["Username"].toString();
        ui->name->setText(USERNAME);
        //qDebug() << "==========" <<USERNAME;
    }
//    qDebug() << "----------------" << data;

//    QString touxiangurl = dcenter.getUser(USERNAME).getTouxiang();
//    qDebug() << "-" << touxiangurl;
//    int lenth = touxiangurl.length();
//    if(lenth > 10){
//        IsChosen = false;
//        QImage image(touxiangurl);
//        QPixmap pixmap = QPixmap::fromImage(image);
//        QPixmap fitpixmap = pixmap.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//        ui->profile->setPixmap(fitpixmap);
//    }else{
      IsChosen = false;
      ui->profile->setPixmap(PaintHead(USERNAME[0]));
//    }

}

// GROUP
Kuang::Kuang(QJsonObject data,QWidget *parent):
    QWidget(parent),
    ui(new Ui::Kuang){
    ui->setupUi(this);
    ui->lblNewMessage->hide();
    SessionID = data["SessionID"].toInt();
    IsChosen = false;
    USERNAME = data["Profile"].toObject()["SessionName"].toString();
    ui->name->setText(USERNAME);
    ui->profile->setPixmap(PaintHead(USERNAME[0]));
}

void Kuang::Showreddot(){
    ui->lblNewMessage->show();
}

void Kuang::Hidereddot(){
    ui->lblNewMessage->hide();
}

Kuang::Kuang(QString UserName,QString NikeName,QWidget *parent):
    QWidget(parent),
    ui(new Ui::Kuang){
    ui->setupUi(this);
    ui->lblNewMessage->hide();
    SessionID = 0;
    IsChosen = false;
    USERNAME = UserName;
    ui->name->setText(UserName);
    ui->profile->setPixmap(PaintHead(USERNAME[0]));
}

Kuang* Kuang::KuangChosenNow = nullptr;

bool Kuang::IsShow = true;

void Kuang::mousePressEvent(QMouseEvent *ev){
//    qDebug() << "Mouse Pressed" << ClientDataCenter::Singleton().getSession(SessionID).getLatestMessageID();
    if(ev->button()==Qt::LeftButton){
        if(!KuangChosenNow || KuangChosenNow != this){
            KuangChosenNow = this;
            emit KuangChosenChanged(this);
        }
    }
}

Kuang::~Kuang()
{
    delete ui;
}
