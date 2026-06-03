#include "loginform.h"
#include "ui_loginform.h"
#include <QDebug>
#include <QMessageBox>
#include <QRegularExpression>

LogInForm::LogInForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogInForm)
{
    ui->setupUi(this);
    qDebug() << "LogInForm created";
    ui->input_password->setEchoMode(QLineEdit::Password);
}

LogInForm::~LogInForm()
{
    qDebug() << "LogInForm destroyed";
    delete ui;
}

bool LogInForm::validateInput(QString login, QString password)
{
    // Проверка на пустой логин
    if (login.isEmpty()) {
        QMessageBox::warning(this,
                           "Ошибка входа",
                           "Логин не может быть пустым!\nПожалуйста, введите ваш логин.");
        ui->input_login->setFocus();
        return false;
    }

    // Проверка на пустой пароль
    if (password.isEmpty()) {
        QMessageBox::warning(this,
                           "Ошибка входа",
                           "Пароль не может быть пустым!\nПожалуйста, введите ваш пароль.");
        ui->input_password->setFocus();
        return false;
    }

    return true;
}

void LogInForm::on_buttonBox_accepted()
{
    QString login = ui->input_login->text().trimmed();
    QString password = ui->input_password->text();

    qDebug() << "Login attempt - Login:" << login;

    if (validateInput(login, password)) {
        // Все проверки пройдены, отправляем сигнал
        qDebug() << "Input validation passed, emitting loginAttempt signal";
        emit loginAttempt(login, password);
    }
}

void LogInForm::on_buttonBox_rejected()
{

    qDebug() << "Login cancelled";

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Отмена входа",
        "Вы уверены, что хотите отменить вход?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        close();
    }
}

void LogInForm::clearLoginField()
{
    if (ui->input_login) {
            ui->input_login->clear();
            ui->input_login->setFocus();
    }
}

void LogInForm::clearPasswordField()
{
    if (ui->input_password) {
            ui->input_password->clear();
            ui->input_password->setFocus();
    }
}

void LogInForm::clearAllFields()
{
    ui->input_login->clear();
    ui->input_password->clear();
    ui->input_login->setFocus();
}

void LogInForm::on_to_regButton_clicked()
{
    qDebug() << "Switch to registration requested";
    emit switchToRegistration();
}
