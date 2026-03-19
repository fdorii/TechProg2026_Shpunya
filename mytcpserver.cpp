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

    // Берём буфер для этого клиента
    QString &res = mBuffers[socket];

    // Читаем все доступные байты и добавляем в буфер клиента
    QByteArray array = socket->readAll();
    qDebug() << "IN: " << array;
    res.append(array);

    // Проверяем буфер на наличие маркера конца команды (\x01).
    // Одна readAll() может содержать несколько команд подряд — обрабатываем все.
    while (res.contains(QChar('\x01')))
    {
        int markerPos = res.indexOf(QChar('\x01'));
        QString command = res.left(markerPos).trimmed();
        res.remove(0, markerPos + 1);  // убираем команду и маркер из буфера

        if (!command.isEmpty())
        {
            QByteArray result = parsing(command);
            qDebug() << "OUT: " << result;
            socket->write(result);
        }
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