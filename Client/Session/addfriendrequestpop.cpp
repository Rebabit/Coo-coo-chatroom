#include "addfriendrequestpop.h"
#include "ui_addfriendrequestpop.h"

AddFriendRequestPop::AddFriendRequestPop(QWidget *parent, QString name) :
    QWidget(parent),
    ui(new Ui::AddFriendRequestPop)
{
    ui->setupUi(this);
    ui->textBrowser->setAlignment(Qt::AlignCenter);
    ui->textBrowser->setText(name+" requests to add you as a friend");

}

AddFriendRequestPop::~AddFriendRequestPop()
{
    delete ui;
}



void AddFriendRequestPop::on_pushButton_clicked()
{
    this->hide();
}

void AddFriendRequestPop::on_pushButton_2_clicked()
{
    this->hide();
}
