#ifndef MSGSESSION_H
#define MSGSESSION_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include <QMouseEvent>


namespace Ui {
class MsgSession;
}

class MsgSession : public QWidget
{
    Q_OBJECT

public:
    explicit MsgSession(QWidget *parent = 0);
    MsgSession(const QString &username, QJsonObject data, QWidget *parent = nullptr);
    MsgSession(QJsonObject data, QWidget *parent = nullptr);
    MsgSession(QString UserName, QString Nicname, QWidget * parent = nullptr);
    void Showreddot();
    void Hidereddot();
    virtual void mousePressEvent(QMouseEvent *ev);
    QPixmap PaintHead(const QChar &name);
    ~MsgSession();
    int SessionID;
    static MsgSession * MsgSessionChosenNow;
    bool IsChosen = false;
    QString USERNAME;
    static bool IsShow;



signals:
    void msgSessionChanged(MsgSession * This);

private:
    Ui::MsgSession *ui;
};

#endif // MSGSESSION_H
