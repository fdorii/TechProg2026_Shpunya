#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

#include <QtNetwork>
#include <QByteArray>
#include <QDebug>

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);
    ~MyTcpServer();
public slots:
    void slotNewConnection();
    void slotClientDisconnected();
    void slotServerRead();
private:
    QTcpServer *mTcpServer;
    QList<QTcpSocket *> mClients;          // список всех подключённых клиентов
    QMap<QTcpSocket *, QString> mBuffers;  // буфер данных для каждого клиента
};
#endif // MYTCPSERVER_H
