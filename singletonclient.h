#ifndef SINGLETONCLIENT_H
#define SINGLETONCLIENT_H

#include <QTcpSocket>
#include <QtNetwork>
#include <QString>
#include <QObject>
#include <QDebug>
#include <QByteArray>

class SingletonClient : public QObject
{
    Q_OBJECT

private:
    static SingletonClient* p_instance;
    QTcpSocket* mTcpSocket;
    QString m_lastMessage;
    QByteArray m_readBuffer;  // Буфер для накопления данных

    explicit SingletonClient(QObject *parent = nullptr);
    SingletonClient(const SingletonClient&) = delete;
    SingletonClient& operator=(SingletonClient&) = delete;
    ~SingletonClient();


public:
    static SingletonClient* getInstance();
    static void destroyInstance();

    void send_msg_to_server(QString query);
    void reconnect();
    bool isConnected() const;
    QString getLastMessage() const;

signals:
    void msg_from_server(QString msg);
    void connected();
    void disconnected();
    void errorOccurred(QString error);

private slots:
    void slotServerRead();
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);
};

#endif // SINGLETONCLIENT_H
