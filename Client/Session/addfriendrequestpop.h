#ifndef ADDFRIENDREQUESTPOP_H
#define ADDFRIENDREQUESTPOP_H

#include <QWidget>

namespace Ui {
class AddFriendRequestPop;
}

class AddFriendRequestPop : public QWidget
{
    Q_OBJECT

public:
    explicit AddFriendRequestPop(QWidget *parent = nullptr, QString name = "");
    ~AddFriendRequestPop();

private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::AddFriendRequestPop *ui;
};

#endif // ADDFRIENDREQUESTPOP_H
