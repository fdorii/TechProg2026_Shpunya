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
            this, &ClientFunctions::handleServerMessage);
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

void ClientFunctions::sendAuthToServer(QString login, QString password, bool isRegistration)
{
    // Формат для сервера
    QString command = isRegistration ? "register" : "login";
    QString message = QString("%1 %2 %3").arg(command, login, password);

    qDebug() << "Sending to server:" << message;
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

        // Закрываем окно регистрации
        if (ui_reg) {
            ui_reg->hide();
        }

        // Открываем главное окно
        qDebug() << "Registration complete, opening main window";

        if (!ui_main) {
            ui_main = new MainWindow();
            ui_main->setAttribute(Qt::WA_DeleteOnClose);

            // Подключаем сигналы от клиента к главному окну
            connect(client, &SingletonClient::msg_from_server,
                    ui_main, &MainWindow::appendServerMessage);
            connect(client, &SingletonClient::connected,
                    ui_main, [this]() { ui_main->updateConnectionStatus(true); });
            connect(client, &SingletonClient::disconnected,
                    ui_main, [this]() { ui_main->updateConnectionStatus(false); });

            // При закрытии главного окна показываем окно регистрации
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

        // Закрываем окно входа
        if (ui_login) ui_login->hide();

        // Показываем главное окно
        if (!ui_main) {
            ui_main = new MainWindow();
            ui_main->setAttribute(Qt::WA_DeleteOnClose);

            // Подключаем сигналы от клиента к главному окну
            connect(client, &SingletonClient::msg_from_server,
                    ui_main, &MainWindow::appendServerMessage);
            connect(client, &SingletonClient::connected,
                    ui_main, [this]() { ui_main->updateConnectionStatus(true); });
            connect(client, &SingletonClient::disconnected,
                    ui_main, [this]() { ui_main->updateConnectionStatus(false); });

            connect(ui_main, &MainWindow::destroyed, this, [this]() {
                ui_main = nullptr;
                qDebug() << "Main window closed, showing login form";
                showLogin();
            });
        }

        ui_main->show();

        // Показываем сообщение об успешном входе
        QMessageBox::information(ui_main,
                                "Вход выполнен",
                                QString("Добро пожаловать, %1!").arg(login));

    } else {
        qDebug() << "Login failed: Invalid credentials";

        // Проверяем, что именно не так
        if (!registeredUsers.contains(login)) {
            // Аккаунт не найден
            QMessageBox::critical(ui_login,
                                "Ошибка входа",
                                QString("Аккаунт с логином \"%1\" не найден!\n\n"
                                       "Проверьте правильность ввода логина или "
                                       "зарегистрируйте новый аккаунт.").arg(login));

            // Предлагаем перейти к регистрации
            QMessageBox::StandardButton reply = QMessageBox::question(
                ui_login,
                "Регистрация",
                "Хотите зарегистрировать новый аккаунт?",
                QMessageBox::Yes | QMessageBox::No
            );

            if (reply == QMessageBox::Yes) {
                showRegistration();
            } else {
                // Очищаем поля и ставим фокус на логин
                // Нужно добавить методы в LogInForm
            }

        } else {
            // Неверный пароль
            QMessageBox::critical(ui_login,
                                "Ошибка входа",
                                "Неверный пароль!\n\n"
                                "Проверьте правильность ввода пароля.");

            // Очищаем поле пароля и ставим фокус
            if (ui_login) {
                // Добавим методы очистки полей в LogInForm
            }
        }

        emit serverMessage("Login failed: Invalid credentials");
    }
}

void ClientFunctions::handleServerMessage(QString msg)
{
    qDebug() << "Server message received:" << msg;
    emit serverMessage(msg);

    // Убираем лишние пробелы
    msg = msg.trimmed();

    // Проверяем ответ сервера
    if (msg == "error" || msg.contains("error", Qt::CaseInsensitive)) {
        qDebug() << "Server returned error!";
        emit serverMessage("Server error: Invalid request");
        return;
    }

    if (msg == "success" || msg.contains("success", Qt::CaseInsensitive) ||
        msg.contains("ok", Qt::CaseInsensitive)) {
        qDebug() << "Server operation successful";
        emit serverMessage("Server: Operation successful");
        return;
    }
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
