#include "friendnotice.h"
#include "ui_friendnotice.h"

FriendNotice::FriendNotice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendNotice)
{
    ui->setupUi(this);
}

FriendNotice::~FriendNotice()
{
    delete ui;
}
