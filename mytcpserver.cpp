#include "mytcpserver.h"
#include "func2serv.h"
#include <QDebug>
#include <QCoreApplication>
#include <QString>

MyTcpServer::~MyTcpServer()
{
    // Закрываем все клиентские сокеты
    for (QTcpSocket *socket : mClients) {
        socket->close();
    }
    mTcpServer->close();
}

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    mTcpServer = new QTcpServer(this);

    connect(mTcpServer, &QTcpServer::newConnection,
            this, &MyTcpServer::slotNewConnection);

    if (!mTcpServer->listen(QHostAddress::Any, 33333)) {
        qDebug() << "server is not started";
    } else {
        qDebug() << "server is started";
    }
}

void MyTcpServer::slotNewConnection()
{
    QTcpSocket *socket = mTcpServer->nextPendingConnection();

    // Добавляем нового клиента в список и инициализируем его буфер
    mClients.append(socket);
    mBuffers[socket] = "";

    socket->write("Hello, World!!! I am echo server!\r\n");

    qDebug() << "New client connected. Total clients:" << mClients.size();

    connect(socket, &QTcpSocket::readyRead,
            this, &MyTcpServer::slotServerRead);
    connect(socket, &QTcpSocket::disconnected,
            this, &MyTcpServer::slotClientDisconnected);
}

void MyTcpServer::slotServerRead()
{
  // Определяем, какой именно клиент прислал данные
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket) return;
    
    // Берем буфер для этого клиента 
    QString &res = mBuffers[socket];

    while (socket->bytesAvailable() > 0)
    {
        QByteArray array = socket->readAll();
        qDebug() << "IN: " << array;
        
        if (array == "\x01")
        {
            // Отправляем накопленный буфер
            QByteArray result = parsing(res.trimmed());
            qDebug() << "OUT: " << result;
            socket->write(result);
            res.clear();  // очищаем буфер
        }
        else
        {
            // Добавляем данные в буфер
            res.append(array);
        }
    }
    
    // Отправляем остаток данных, если есть
    if (!res.isEmpty()) {
        QByteArray result = parsing(res.trimmed());
        qDebug() << "OUT: " << result;
        socket->write(result);
        res.clear();
    }
}

void MyTcpServer::slotClientDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket) return;

    qDebug() << "Client disconnected. Total clients:" << mClients.size() - 1;

    mClients.removeAll(socket);
    mBuffers.remove(socket);

    socket->close();
    socket->deleteLater(); // безопасное удаление объекта
}
