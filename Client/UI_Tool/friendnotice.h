#ifndef FRIENDNOTICE_H
#define FRIENDNOTICE_H

#include <QWidget>

namespace Ui {
class FriendNotice;
}

class FriendNotice : public QWidget
{
    Q_OBJECT

public:
    explicit FriendNotice(QWidget *parent = nullptr);
    ~FriendNotice();

private:
    Ui::FriendNotice *ui;
};

#endif // FRIENDNOTICE_H
