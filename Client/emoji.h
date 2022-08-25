#ifndef EMOJI_H
#define EMOJI_H

#include <QWidget>

namespace Ui {
class emoji;
}

class emoji : public QWidget
{
    Q_OBJECT

public:
    explicit emoji(QWidget *parent = 0);
    emoji(QString username,QWidget *parent = 0);
    ~emoji();
    void addEmoji();
    void initConnect();
    void oncellClicked(int row,int column);
signals:
    void insertEmoji(int num);
    void charMsg(QString username, QString fileURL);
public slots:
    void show_product_sets(int row,int col);
private:
    Ui::emoji *ui;
    QList<QString> m_emotionList;
    QString username;
};

#endif // EMOJI_H
