#include "handlesignal.h"

handleSignal::handleSignal(QObject *parent) : QObject(parent)
{

}

handleSignal::handleSignal(int handle)
{
    emit handleSignal::sendSignal (handle);
}

void handleSignal::aaa(int handle)
{
    emit handleSignal::sendSignal (handle);
}
