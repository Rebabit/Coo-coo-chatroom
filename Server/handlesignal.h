#ifndef HANDLESIGNAL_H
#define HANDLESIGNAL_H

#include <QObject>

class handleSignal : public QObject
{
    Q_OBJECT
public:
    explicit handleSignal(QObject *parent = nullptr): QObject(parent){}
    handleSignal(int);
    void emitSignal(int);

signals:
    void sendSignal(int);

};

#endif // HANDLESIGNAL_H
