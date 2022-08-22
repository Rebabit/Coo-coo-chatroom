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

    QList<QJsonObject> registerResponse(QJsonObject json);
    QList<QJsonObject> loginResponse(QJsonObject json);
    QList<QJsonObject> newMessageResponse(QJsonObject json);
    void addFriendResponse(QJsonObject json);
    QList<QJsonObject> searchResponse(QJsonObject json);
    void createGroupResponse(QJsonObject json);
};

#endif // OPERATIONS_H
