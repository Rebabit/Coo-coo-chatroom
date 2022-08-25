#ifndef KUANG_H
#define KUANG_H

#include <QWidget>
#include<QJsonObject>
#include<QString>
#include<QJsonArray>
#include<QMouseEvent>

namespace Ui {
class Kuang;
}

class Kuang : public QWidget
{
    Q_OBJECT

public:
    explicit Kuang(QWidget *parent = nullptr);
    Kuang(const QString &username,QJsonObject data,QWidget *parent = nullptr);
    Kuang(QJsonObject data,QWidget *parent = nullptr);
    Kuang(QString UserName,QString NikeName,QWidget *parent = nullptr);
    void Showreddot();
    void Hidereddot();
    virtual void mousePressEvent(QMouseEvent *ev);
    QPixmap PaintHead(const QChar &name);
    ~Kuang();
    int SessionID;
    bool IsChosen = false;
    QString USERNAME;
    static Kuang *KuangChosenNow;
    static bool IsShow;

signals:
    void KuangChosenChanged(Kuang *This);

private:
    Ui::Kuang *ui;
};

#endif // KUANG_H
