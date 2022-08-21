#include "handlesignal.h"

handleSignal::handleSignal(int handle)
{
    emitSignal(handle);
}

void handleSignal::emitSignal(int handle)
{
    emit handleSignal::sendSignal (handle);
}
