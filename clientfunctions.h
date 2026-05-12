#ifndef CLIENTFUNCTIONS_H
#define CLIENTFUNCTIONS_H

#include <QObject>
#include <QMap>

// Forward declarations
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
    void openMainWindow();
    void handleServerConnected();
    void handleServerDisconnected();
    void handleServerError(QString error);
    void onServerMessage(const QString &msg);

signals:
    void auth_ok();
    void serverMessage(QString msg);

private:
    enum class AuthState { None, Registering, LoggingIn };
    AuthState m_authState = AuthState::None;
    MainWindow *ui_main = nullptr;
    RegistrationForm *ui_reg = nullptr;
    LogInForm *ui_login = nullptr;
    SingletonClient *client = nullptr;

    QString m_pendingLogin;
    QString m_pendingPassword;

    QMap<QString, QString> registeredUsers;

    void setupClientConnections();
    void sendToServer(const QString &type, const QString &login, const QString &password);
    void processAuthResponse(bool success, const QString &message);
};

#endif // CLIENTFUNCTIONS_H
