#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <QObject>
#include <QJsonObject>
#include <QList>

class Operations : public QObject
{
    Q_OBJECT
    using resp = QList<QJsonObject>;
    using Json = QJsonObject;

public:
    static Operations& Singleton(QObject *parent = nullptr)
    {
        static Operations * singleton = new Operations(parent);
        return *singleton;
    }

    QList<QJsonObject> request(QJsonObject json);

signals:
    void newMessage(int sessionId, QJsonObject msg);

public:
    explicit Operations(QObject *parent = nullptr);
    QList<QJsonObject> huantouResponse(QJsonObject json);
    QList<QJsonObject> registerResponse(QJsonObject json);
    QList<QJsonObject> loginResponse(QJsonObject json);
    QList<QJsonObject> newMessageResponse(QJsonObject json);
    const QJsonObject addFriendResponse(QJsonObject json);
    QList<QJsonObject> searchResponse(QJsonObject json);
    void createGroupResponse(QJsonObject json);
    const QJsonObject addFriendResultResponse(QJsonObject json);
    QList<QJsonObject> changeSignatureResponse(QJsonObject json);
};

#endif // OPERATIONS_H
