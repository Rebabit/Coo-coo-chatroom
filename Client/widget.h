#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QtNetwork/QNetworkRequest>
#include<QtNetwork/QNetworkReply>
#include<QEventLoop>
#include<QListWidget>
#include<QJsonValue>
#include<codeeditor.h>


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QByteArray get(const QString &str_url){

        const QUrl url = QUrl::fromUserInput(str_url);
        QNetworkRequest qnr(url);
        QNetworkAccessManager qnam;
        QNetworkReply *reply = qnam.get(qnr);
        QEventLoop eventloop;
        QObject::connect(reply, &QNetworkReply::finished, &eventloop, &QEventLoop::quit);
        eventloop.exec(QEventLoop::ExcludeUserInputEvents);
        QByteArray reply_data = reply->readAll();
        reply->deleteLater();
        reply = nullptr;
        return reply_data;
    }


private slots:
    void on_pushButton_clicked();
    void showContents(QListWidgetItem*);

private:
    Ui::Widget *ui;
    QJsonValue arrayValue;
    CodeEditor *configEditor;
};


int captureGitRepo(QString raw, QList<QString> &authors, QList<QString> &repos, QList<QString> &commits);
QString generateGitMessage(QString owner, QString repo, QString sha);
#endif // WIDGET_H
