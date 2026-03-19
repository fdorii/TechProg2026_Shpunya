#ifndef SINGLETONCLIENT_H
#define SINGLETONCLIENT_H
#include <QTcpSocket>
#include <QtNetwork>
#include <QString>
#include <QObject>
#include <QDebug>
#include <QByteArray>

class SingletonClient;

class SingletonDestroyer
{
    private:
        SingletonClient * p_instance;
    public:
        ~SingletonDestroyer() { delete p_instance;}
        void initialize(SingletonClient * p){p_instance = p;}
};


class SingletonClient: public QObject
{
    Q_OBJECT
    private:
        static SingletonClient * p_instance;
        static SingletonDestroyer destroyer;
        QTcpSocket * mTcpSocket;
    protected:
        explicit SingletonClient(QObject *parent = nullptr);
        SingletonClient(const SingletonClient& ) = delete;
        SingletonClient& operator = (SingletonClient &) = delete;
        ~SingletonClient();
        friend class SingletonDestroyer;
    public:
        static SingletonClient* getInstance();
        void send_msq_to_server(QString query);
    signals:
        void msg_from_server(QString msg);
    private slots:
        void ServerRead();
};

#endif // SINGLETONCLIENT_H
