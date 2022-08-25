#include "emoji.h"
#include "ui_emoji.h"
#include "mainwindow.h"
#include<QHeaderView>
#include<QLabel>
#include<QDebug>
#include<QDir>
#include<QTableWidget>

emoji::emoji(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::emoji)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setMouseTracking(true);
    ui->tableWidget->setShowGrid(false);
    addEmoji();
    initConnect();
}

emoji::emoji(QString user,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::emoji),username(user)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setMouseTracking(true);
    ui->tableWidget->setShowGrid(false);
    addEmoji();
    initConnect();
}

emoji::~emoji()
{
    delete ui;
}

//添加表情
void emoji::addEmoji()
{
    setMouseTracking(true);
    int columncount = 7;
    int emojiIndex = 0;
    int rowIndex = 0;
    int columnIndex = 0;

    QString path = "../../Client/Resources/emotion/%1.png";
    QDir dir;
    QString pathname;
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    pathname = dir.currentPath();
    qDebug()<< "pathname:" <<pathname;
//    qDebug() << "Here emoji ";
    //auto it = code2url.begin();
    for (emojiIndex = 0; emojiIndex < 32; emojiIndex++){
        rowIndex = emojiIndex / columncount;
        columnIndex = emojiIndex % columncount;
        QLabel * label = new QLabel(this);
//        qDebug() << path.arg(emojiIndex + 1) << " ";
        label->setPixmap(QPixmap(path.arg(emojiIndex + 1)));
        label->setScaledContents(true);
        label->setAlignment(Qt::AlignCenter);
        //label->setToolTip(it.key());
        ui->tableWidget->setCellWidget(rowIndex, columnIndex, label);
        //++emojiIndex;
        //++it;
    }

}


void emoji::show_product_sets(int row,int column)
{
    if (row<0 || column<0 || row>5 || column>7){
        return;
    }
    QLabel * label = dynamic_cast<QLabel *>(ui->tableWidget->cellWidget(row, column));
    if (label != Q_NULLPTR){
        label->setStyleSheet("background-color:white");
        hide();
        QString path = "../../Client/Resources/emotion/%1.png";
        qDebug() << "==========insertEmoji!===========" << row << column << path.arg(row*7+column+1);
        emit charMsg(username, path.arg(row*7+column+1));
        //emit insertEmoji((row-1)*7+column);
    }
}

void emoji::initConnect()
{
//    connect(ui->tableWidget, cellClicked(int,int), this, &emoji::oncellClicked);
    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(show_product_sets(int,int)));
}
