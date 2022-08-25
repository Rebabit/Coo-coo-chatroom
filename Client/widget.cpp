#include "widget.h"
#include "ui_widget.h"
#include<QJsonObject>
#include<QJsonDocument>
#include<QJsonArray>
#include<QByteArray>
#include<QTextBrowser>
#include<QApplication>
#include<QTextDocument>
#include "myhighlighter.h"
#include "codeeditor.h"
#include "typedef.h"
#include <QGridLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    configEditor = new CodeEditor();
    configEditor->setMode(BROWSE);
    ui->gridLayout_3->addWidget(configEditor);
    connect (ui->listWidget, &QListWidget::itemDoubleClicked, this, &Widget::showContents);
}

Widget::~Widget()
{
    delete ui;

}


void Widget::on_pushButton_clicked()
{
    QByteArray file = get("https://gitee.com/api/v5/repos/" + this->ui->lineEdit->text() + "/" + this->ui->lineEdit_2->text() + "/git/trees/master");

    QJsonDocument newjson = QJsonDocument::fromJson(file);
    QJsonObject jsonObject = newjson.object ();

    arrayValue = jsonObject.value(QStringLiteral("tree"));
    int length = 0;
    if(arrayValue.isArray ())
    {
        QJsonArray array = arrayValue.toArray();
        length = array.size ();
        for(int i=0;i<length;i++)
        {
            QJsonValue iconArray = array.at(i);
            QJsonObject icon = iconArray.toObject();
            ui->listWidget->addItem (icon["path"].toString ());
        }
    }

}

void Widget::showContents(QListWidgetItem * nowItem)
{


    if(arrayValue.isArray ())
    {
        QJsonArray array = arrayValue.toArray();
        for(int i=0;i<array.size();i++)
        {
            QJsonValue iconArray = array.at(i);
            QJsonObject icon = iconArray.toObject();

            if(icon["path"].toString ()==nowItem->text ())
            {
                QByteArray content = get(icon["url"].toString ());
                QJsonDocument newjson = QJsonDocument::fromJson(content);
                QJsonObject jsonObject = newjson.object ();

                configEditor->setPlainText(QByteArray::fromBase64(jsonObject["content"].toString ().toUtf8 ()).data ());
                //ui->gridLayout->addWidget(configEditor);
                QString text = "def hhh()";
                MyHighLighter *highlighter = new MyHighLighter(configEditor->document());

                //Highlighter * h = new Highlighter(ui->textEdit->document ());

            }

        }
    }
}

// git commit try at:   https://gitee.com/api/v5/repos/jasonliu233/gitstudy/commits/master
// git tree at:    https://gitee.com/api/v5/repos/jasonliu233/zhishigongcheng/git/trees/master

int captureGitRepo(QString raw, QList<QString> &authors, QList<QString> &repos, QList<QString> &commits) {
    QRegExp reg("@\\[((?:[^ ,\\,])+),[ ]*((?:[^ ,\\,])+)(?:,[ ]*((?:[^ ,\\,])*))?\\]");
    int pos = 0, initial = authors.size();
    while ((pos = reg.indexIn(raw, pos)) != -1) {
        authors.append(reg.cap(1));
        repos.append(reg.cap(2));
        commits.append(reg.cap(3) == "" ? "master" : reg.cap(3));
        pos += reg.matchedLength();
    }
    return authors.size() - initial;
}

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

QString generateGitMessage(QString owner, QString repo, QString sha) {
    QString url = "https://gitee.com/api/v5/repos/"+owner+"/"+repo+"/"+"commits"+"/"+sha;

    QByteArray file = get(url);

    QJsonDocument newjson = QJsonDocument::fromJson(file);
    QJsonObject jsonObject = newjson.object ();

    QString output;
    QString msg = jsonObject["commit"].toObject ()["message"].toString ();
    QString author = jsonObject["commit"].toObject ()["author"].toObject ()["name"].toString ();
    QString commitInformation = "提交信息 git@gitee:" + owner + "/" + repo + "\n" + msg+"by "+author + "\n";

    output.append (commitInformation);
    QJsonArray fileList = jsonObject["files"].toArray ();
    for(int i=0;i<fileList.size ();i++)
    {
        QJsonObject js = fileList[i].toObject ();
        QString fileName = js["filename"].toString ();
        int add = js["additions"].toInt ();
        int del = js["deletions"].toInt ();
        QString addS;
        for(int i=0;i<add;i++)
            addS.append ("+");
        QString delS;
        for(int i=0;i<del;i++)
            delS.append ("-");
        QString fileInformation = "    " + fileName + " " + addS + delS + "\n";
        output.append (fileInformation);
    }
    qDebug()<< output;
    return output;
}
