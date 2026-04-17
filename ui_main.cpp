#include <QApplication>
#include <QString>
#include "clientfunctions.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    new ClientFunctions();
    a.setQuitOnLastWindowClosed(true);

    return a.exec();
}
