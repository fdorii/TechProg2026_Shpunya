#include "clientfunctions.h"
#include "registrationform.h"
#include "loginform.h"
#include "mainwindow.h"
#include "statisticwindow.h"
#include "singletonclient.h"
#include <QDebug>
#include <QMessageBox>
#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>

ClientFunctions::ClientFunctions(QObject *parent)
    : QObject(parent)
{
    qDebug() << "ClientFunctions constructor called";

    client = SingletonClient::getInstance();
    setupClientConnections();

    showRegistration();
}

ClientFunctions::~ClientFunctions()
{
    qDebug() << "ClientFunctions destructor called";

    if (ui_reg) ui_reg->deleteLater();
    if (ui_login) ui_login->deleteLater();
    if (ui_main) ui_main->deleteLater();
}

void ClientFunctions::setupClientConnections()
{
    if (!client) return;

    // сигналы от клиента
    connect(client, &SingletonClient::msg_from_server,
            this, &ClientFunctions::handleServerMessage);
    connect(client, &SingletonClient::connected,
            this, &ClientFunctions::handleServerConnected);
    connect(client, &SingletonClient::disconnected,
            this, &ClientFunctions::handleServerDisconnected);
    connect(client, &SingletonClient::errorOccurred,
            this, &ClientFunctions::handleServerError);
}

void ClientFunctions::sendAuthToServer(QString login, QString password, bool isRegistration)
{
    // JSON сообщение для сервера
    QJsonObject json;
    json["type"] = isRegistration ? "register" : "login";
    json["login"] = login;
    json["password"] = password;

    QJsonDocument doc(json);
    QString message = doc.toJson(QJsonDocument::Compact);

    client->send_msg_to_server(message);
}

void ClientFunctions::authorized(QString login, QString password)
{
    qDebug() << "Registration - Login:" << login;

    if (!login.isEmpty() && !password.isEmpty()) {
        // Сохраняем пользователя локально
        registeredUsers[login] = password;
        qDebug() << "User registered locally. Total users:" << registeredUsers.size();

        // Отправляем данные на сервер
        sendAuthToServer(login, password, true);

        if (ui_reg) {
            ui_reg->hide();
        }

        qDebug() << "Registration complete, opening main window";

        if (!ui_main) {
            ui_main = new MainWindow();
            ui_main->setAttribute(Qt::WA_DeleteOnClose);

            connect(ui_main, &MainWindow::destroyed, this, [this]() {
                ui_main = nullptr;
                qDebug() << "Main window closed, showing registration form";
                showRegistration();
            });
        }

        ui_main->show();
        emit auth_ok();
    }
}

void ClientFunctions::handleLoginAttempt(QString login, QString password)
{
    qDebug() << "Login attempt - Login:" << login;

    // Проверяем локально
    if (registeredUsers.contains(login) && registeredUsers[login] == password) {
        qDebug() << "Login successful locally!";

        // Отправляем на сервер
        sendAuthToServer(login, password, false);

        if (ui_login) ui_login->hide();

        if (!ui_main) {
            ui_main = new MainWindow();
            ui_main->setAttribute(Qt::WA_DeleteOnClose);

            connect(ui_main, &MainWindow::destroyed, this, [this]() {
                ui_main = nullptr;
                qDebug() << "Main window closed, showing login form";
                showLogin();
            });
        }

        ui_main->show();
    } else {
        qDebug() << "Login failed: Invalid credentials";
        emit serverMessage("Login failed: Invalid credentials");
    }
}

void ClientFunctions::handleServerMessage(QString msg)
{
    qDebug() << "Server message received:" << msg;
    emit serverMessage(msg);

    msg = msg.trimmed();

    if (msg == "error" || msg.contains("error", Qt::CaseInsensitive)) {
        qDebug() << "Server returned error!";
        emit serverMessage("Server error: Invalid request");

        if (ui_main) {
            QMessageBox::warning(ui_main, "Server Error",
                               "Server returned an error. Please check your data.");
        }
        return;
    }

    if (msg == "success" || msg.contains("success", Qt::CaseInsensitive) ||
        msg.contains("ok", Qt::CaseInsensitive)) {
        qDebug() << "Server operation successful";
        emit serverMessage("Server: Operation successful");

        if (ui_reg && msg.contains("register", Qt::CaseInsensitive)) {
            QMessageBox::information(nullptr, "Success", "Registration successful!");
        }
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(msg.toUtf8());
    if (doc.isObject()) {
        QJsonObject json = doc.object();
        QString status = json["status"].toString();
        QString message = json["message"].toString();

        if (status == "success") {
            qDebug() << "Server: Success -" << message;
        } else {
            qDebug() << "Server: Error -" << message;
        }
    }
}

void ClientFunctions::handleServerConnected()
{
    qDebug() << "Connected to server!";
    emit serverMessage("Connected to server");

    // Если есть главное окно, обновляем статус
    if (ui_main) {
        // Можно добавить метод updateConnectionStatus в MainWindow
        // ui_main->updateConnectionStatus(true);
    }
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
