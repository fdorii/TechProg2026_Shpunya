#include <QApplication>
#include <QString>
#include "clientfunctions.h"
#include "singletonclient.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "Starting application...";
    new ClientFunctions();

    a.setQuitOnLastWindowClosed(true);

    int result = a.exec();

    SingletonClient::destroyInstance();

    qDebug() << "Application finished with code:" << result;
    return result;
}
