#include "widget.h"

#include <QApplication>
#include "ltest.h"
#include "usermodel.h"

#include "testcases.cpp"

//#define TESTMODE

int main(int argc, char *argv[])
{
#ifdef TESTMODE
    return RunAllTests();
#endif
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
