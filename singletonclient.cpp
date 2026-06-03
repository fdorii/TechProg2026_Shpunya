#include "singletonclient.h"

SingletonClient* SingletonClient::p_instance = nullptr;

SingletonClient::SingletonClient(QObject *parent)
    : QObject(parent)
    , mTcpSocket(nullptr)
{
    qDebug() << "SingletonClient constructor called";

    mTcpSocket = new QTcpSocket(this);

    connect(mTcpSocket, &QTcpSocket::readyRead, this, &SingletonClient::slotServerRead);
    connect(mTcpSocket, &QTcpSocket::connected, this, &SingletonClient::onConnected);
    connect(mTcpSocket, &QTcpSocket::disconnected, this, &SingletonClient::onDisconnected);
    connect(mTcpSocket, &QTcpSocket::errorOccurred, this, &SingletonClient::onError);

    qDebug() << "Connecting to server...";
    mTcpSocket->connectToHost("127.0.0.1", 33333);
}

SingletonClient::~SingletonClient()
{
    qDebug() << "SingletonClient destroyed";
    if (mTcpSocket) {
        mTcpSocket->disconnectFromHost();
        if (mTcpSocket->state() != QAbstractSocket::UnconnectedState) {
            mTcpSocket->waitForDisconnected(1000);
        }
        mTcpSocket->deleteLater();
    }
    p_instance = nullptr;
}

SingletonClient* SingletonClient::getInstance()
{
    if (!p_instance) {
        p_instance = new SingletonClient();
    }
    return p_instance;
}

void SingletonClient::destroyInstance()
{
    if (p_instance) {
        delete p_instance;
        p_instance = nullptr;
    }
}

void SingletonClient::send_msg_to_server(QString query)
{
    if (mTcpSocket && mTcpSocket->state() == QTcpSocket::ConnectedState) {
        QByteArray data = query.toUtf8();
        data.append('\x01');
        mTcpSocket->write(data);
        mTcpSocket->flush();
        qDebug() << "Sent to server:" << query;
    } else {
        qDebug() << "Cannot send message: not connected to server";
        emit errorOccurred("Not connected to server");
    }
}

void SingletonClient::slotServerRead()
{
    QByteArray newData = mTcpSocket->readAll();
    qDebug() << "Buffer received:" << newData;

    m_readBuffer.append(newData);

    // Пробуем парсить JSON, если это он
    QJsonDocument doc = QJsonDocument::fromJson(m_readBuffer);
    if (doc.isObject() && doc.object().contains("task1")) {
        QString msg = QString::fromUtf8(m_readBuffer);
        m_lastMessage = msg;
        qDebug() << "JSON stats received:" << msg;
        emit msg_from_server(msg);
        m_readBuffer.clear();
        return;
    }

    // Старый построчный парсинг
    while (m_readBuffer.contains('\n')) {
        int endIndex = m_readBuffer.indexOf('\n');
        QByteArray message = m_readBuffer.left(endIndex).trimmed();
        m_readBuffer.remove(0, endIndex + 1);

        if (!message.isEmpty()) {
            QString msg = QString::fromUtf8(message);
            m_lastMessage = msg;

            if (!msg.contains("Hello, World") && !msg.contains("echo server")) {
                qDebug() << "Message from server:" << msg;
                emit msg_from_server(msg);
            } else {
                qDebug() << "Skipping server greeting:" << msg;
            }
        }
    }
}

void SingletonClient::onConnected()
{
    qDebug() << "Connected to server successfully!";
    emit connected();
}

void SingletonClient::onDisconnected()
{
    qDebug() << "Disconnected from server";
    emit disconnected();
}

void SingletonClient::onError(QAbstractSocket::SocketError socketError)
{
    QString errorMsg = mTcpSocket->errorString();
    qDebug() << "Socket error:" << socketError << errorMsg;
    emit errorOccurred(errorMsg);
}

void SingletonClient::reconnect()
{
    if (mTcpSocket) {
        mTcpSocket->abort();
        mTcpSocket->connectToHost("127.0.0.1", 33333);
    }
}

bool SingletonClient::isConnected() const
{
    return mTcpSocket && mTcpSocket->state() == QTcpSocket::ConnectedState;
}

QString SingletonClient::getLastMessage() const
{
    return m_lastMessage;
}
