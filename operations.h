#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <QObject>
#include <QJsonObject>
#include <QList>

class Operations : public QObject
{
    Q_OBJECT

public:
    explicit Operations(QObject *parent() = nullptr):QObject(parent){}
    static Operations& Singleton(QObject *parent = nullptr);
    //QList<QJsonObject> request(QJsonObject json);
    QList<QJsonObject> registerResponse(QJsonObject json);//注册处理
    QList<QJsonObject> loginResponse(QJsonObject json);//登录处理
    QList<QJsonObject> newMessageResponse(QJsonObject json);//消息处理

signals:
    void newMessage(int sessionId, QJsonObject msg);

};

#endif // OPERATIONS_H
