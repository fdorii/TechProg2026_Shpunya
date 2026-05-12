#include "clientfunctions.h"
#include "registrationform.h"
#include "loginform.h"
#include "mainwindow.h"
#include "singletonclient.h"
#include <QDebug>
#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

ClientFunctions::ClientFunctions(QObject *parent)
    : QObject(parent)
{
    qDebug() << "ClientFunctions constructor called";

    // Получаем экземпляр синглтона клиента
    client = SingletonClient::getInstance();
    setupClientConnections();

    // Первым показываем окно регистрации
    showRegistration();
}

ClientFunctions::~ClientFunctions()
{
    qDebug() << "ClientFunctions destructor called";

    // Очищаем окна
    if (ui_reg) ui_reg->deleteLater();
    if (ui_login) ui_login->deleteLater();
    if (ui_main) ui_main->deleteLater();
}

void ClientFunctions::setupClientConnections()
{
    if (!client) return;

    // Подключаем сигналы от клиента
    connect(client, &SingletonClient::msg_from_server,
            this, &ClientFunctions::onServerMessage);
    connect(client, &SingletonClient::connected,
            this, &ClientFunctions::handleServerConnected);
    connect(client, &SingletonClient::disconnected,
            this, &ClientFunctions::handleServerDisconnected);
    connect(client, &SingletonClient::errorOccurred,
            this, &ClientFunctions::handleServerError);

    // Если клиент уже подключен
    if (client->isConnected()) {
        handleServerConnected();
    }
}

void ClientFunctions::sendToServer(const QString &type, const QString &login, const QString &password)
{
    if (!client->isConnected()) {
        QMessageBox::warning(nullptr,
                                   "Connection Error",
                                   "Server is not connected!");
        m_authState = AuthState::None;
        return;
    }

    QString data;

    // Формат для сервера: "команда логин пароль"
    if (type == "register" || type == "reg") {
        data = QString("reg %1 %2").arg(login, password);
    } else if (type == "login" || type == "auth") {
        data = QString("auth %1 %2").arg(login, password);
    } else {
        // Другие команды в JSON формате
        QJsonObject json;
        json["type"] = type;
        json["login"] = login;
        json["password"] = password;
        data = QJsonDocument(json).toJson(QJsonDocument::Compact);
    }

    qDebug() << "[Send to Server]" << data;
    client->send_msg_to_server(data);
}

void ClientFunctions::authorized(QString login, QString password)
{
    qDebug() << "[Registration] Login:" << login;

    if (login.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(nullptr, "Error", "Login or password is empty!");
            return;
        }

    // Сохраняем для ответа сервера
    m_pendingLogin = login;
    m_pendingPassword = password;
    m_authState = AuthState::Registering;

    // Сохраняем локально
    registeredUsers[login] = password;
    qDebug() << "User registered locally. Total users:" << registeredUsers.size();

    // Отправляем данные на сервер
    sendToServer("reg", login, password);

    qDebug() << "Waiting for server response...";
}

void ClientFunctions::handleLoginAttempt(QString login, QString password)
{
    qDebug() << "[Login] Login:" << login;

    if (login.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(nullptr, "Error", "Login or password is empty!");
            return;
        }

    // Сохраняем для ответа сервера
    m_pendingLogin = login;
    m_pendingPassword = password;
    m_authState = AuthState::LoggingIn;

    // Отправляем на сервер
    sendToServer("auth", login, password);

    qDebug() << "Waiting for server response...";
}


void ClientFunctions::onServerMessage(const QString &msg)
{
    qDebug() << "[Server Message]" << msg.trimmed();
    emit serverMessage(msg);

    // Если не ожидаем ответа - просто транслируем сообщение
    if (m_authState == AuthState::None) return;

    const QString response = msg.trimmed();

    // Проверяем успех/неудачу
    bool success = response.contains("auth+") ||
                   response.contains("reg+") ||
                   response.contains("success") ||
                   response.contains("correct");

    processAuthResponse(success, response);
}

void ClientFunctions::handleServerConnected()
{
    qDebug() << "Connected to server!";
    emit serverMessage("Connected to server");
}

void ClientFunctions::handleServerDisconnected()
{
    qDebug() << "Disconnected from server";
    emit serverMessage("Disconnected from server");
}

void ClientFunctions::handleServerError(QString error)
{
    qDebug() << "Server error:" << error;
    emit serverMessage("Server error: " + error);

    // Показываем ошибку пользователю
    QWidget *parent = nullptr;
    if (ui_main) parent = ui_main;
    else if (ui_reg) parent = ui_reg;
    else if (ui_login) parent = ui_login;

    if (parent) {
        QMessageBox::warning(parent, "Connection Error",
                           "Server error: " + error);
    }
}

void ClientFunctions::processAuthResponse(bool success, const QString &message)
{
    qDebug() << "[Process Response] Success:" << success << "Message:" << message;

    if (m_authState == AuthState::Registering) {
        if (success) {
            QMessageBox::information(nullptr,
                                   "Регистрация",
                                   "Регистрация успешно завершена!\nТеперь вы можете войти.");
            showLogin();
        } else {
            QMessageBox::warning(nullptr,
                               "Ошибка регистрации",
                               message.isEmpty() ? "Регистрация не удалась." : message);
            if (ui_reg) ui_reg->show();
        }
    }
    else if (m_authState == AuthState::LoggingIn) {
        if (success) {
            qDebug() << "Login successful!";

            if (!registeredUsers.contains(m_pendingLogin)) {
                registeredUsers[m_pendingLogin] = m_pendingPassword;
            }

            if (ui_login) ui_login->hide();

            openMainWindow();
        } else {
            qDebug() << "Login failed!";
            QMessageBox::warning(nullptr,
                               "Ошибка входа",
                               message.isEmpty() ? "Неверный логин или пароль." : message);

            if (ui_login) {
                ui_login->show();
                ui_login->clearPasswordField();
            }
        }
    }

    m_authState = AuthState::None;
    m_pendingLogin.clear();
    m_pendingPassword.clear();
}

void ClientFunctions::showLogin()
{
    if (!ui_login) {
        ui_login = new LogInForm();
        ui_login->setAttribute(Qt::WA_DeleteOnClose);

        connect(ui_login, &LogInForm::loginAttempt,
                this, &ClientFunctions::handleLoginAttempt);
        connect(ui_login, &LogInForm::switchToRegistration,
                this, &ClientFunctions::showRegistration);

        connect(ui_login, &LogInForm::destroyed, this, [this]() {
            ui_login = nullptr;
            if (!ui_reg && !ui_main) {
                showRegistration();
            }
        });
    }

    if (ui_reg) ui_reg->hide();
    if (ui_main) ui_main->hide();

    ui_login->show();
    qDebug() << "LogInForm shown";
}

void ClientFunctions::showRegistration()
{
    if (!ui_reg) {
        ui_reg = new RegistrationForm();
        ui_reg->setAttribute(Qt::WA_DeleteOnClose);

        connect(ui_reg, &RegistrationForm::authorized,
                this, &ClientFunctions::authorized);
        connect(ui_reg, &RegistrationForm::switchToLogin,
                this, &ClientFunctions::showLogin);

        connect(ui_reg, &RegistrationForm::destroyed, this, [this]() {
            ui_reg = nullptr;
            if (!ui_login && !ui_main) {
                qDebug() << "All windows closed, quitting application";
                qApp->quit();
            }
        });
    }

    if (ui_login) ui_login->hide();
    if (ui_main) ui_main->hide();

    ui_reg->show();
    qDebug() << "RegistrationForm shown";
}

void ClientFunctions::openMainWindow()
{
    qDebug() << "[Open MainWindow]";

    if (!ui_main) {
        ui_main = new MainWindow();
        ui_main->setAttribute(Qt::WA_DeleteOnClose);

        // Подключаем серверные сигналы к главному окну
        connect(client, &SingletonClient::msg_from_server,
                ui_main, &MainWindow::appendServerMessage);
        connect(client, &SingletonClient::connected,
                ui_main, [this]() {
                    if (ui_main) ui_main->updateConnectionStatus(true);
                });
        connect(client, &SingletonClient::disconnected,
                ui_main, [this]() {
                    if (ui_main) ui_main->updateConnectionStatus(false);
                });

        // При закрытии главного окна возвращаемся на вход
        connect(ui_main, &MainWindow::destroyed, this, [this]() {
            ui_main = nullptr;
            qDebug() << "MainWindow closed, returning to login";
            showLogin();
        });
    }

    ui_main->show();

    // Показываем приветствие
    QMessageBox::information(ui_main, "Вход выполнен",
                           QString("Добро пожаловать, %1!").arg(m_pendingLogin));

    emit auth_ok();
}
