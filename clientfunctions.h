#ifndef CLIENTFUNCTIONS_H
#define CLIENTFUNCTIONS_H

#include <QObject>
#include <QMap>

class MainWindow;
class RegistrationForm;
class LogInForm;
class SingletonClient;

class ClientFunctions : public QObject
{
    Q_OBJECT
public:
    explicit ClientFunctions(QObject *parent = nullptr);
    ~ClientFunctions();

public slots:
    void authorized(QString login, QString password);
    void handleLoginAttempt(QString login, QString password);
    void showRegistration();
    void showLogin();
    void handleServerMessage(QString msg);
    void handleServerConnected();
    void handleServerDisconnected();
    void handleServerError(QString error);

signals:
    void auth_ok();
    void serverMessage(QString msg);

private:
    MainWindow *ui_main = nullptr;
    RegistrationForm *ui_reg = nullptr;
    LogInForm *ui_login = nullptr;
    SingletonClient *client = nullptr;

    // Хранилище зарегистрированных пользователей (временное)
    QMap<QString, QString> registeredUsers;

    void setupClientConnections();
    void sendAuthToServer(QString login, QString password, bool isRegistration);
};

#endif // CLIENTFUNCTIONS_H
