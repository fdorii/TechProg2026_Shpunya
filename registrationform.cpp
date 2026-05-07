#include "registrationform.h"
#include "ui_registrationform.h"
#include <QDebug>
#include <QMessageBox>
#include <QRegularExpression>

RegistrationForm::RegistrationForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegistrationForm)
{
    ui->setupUi(this);
    qDebug() << "RegistrationForm created";
}

RegistrationForm::~RegistrationForm()
{
    qDebug() << "RegistrationForm destroyed";
    delete ui;
}

void RegistrationForm::on_buttonBox_accepted()
{
    QString login = ui->input_login->text().trimmed();
    QString password = ui->input_password->text();

    qDebug() << "Registration attempt - Login:" << login;

    if (login.isEmpty()) {
        qDebug() << "Login is empty!";
        QMessageBox::warning(this, "Ошибка регистрации!", "Поле ввода логина не может быть пустым!\nПожалуйста, введите логин.");
        ui->input_login->setFocus();
        return;
    }

    if (password.isEmpty()) {
        qDebug() << "Password is empty!";
        QMessageBox::warning(this, "Ошибка регистрации!", "Поле ввода пароля не может быть пустым!\nПожалуйста, введите пароль.");
        ui->input_login->setFocus();
        return;
    }

    if (password.length() < 6) {
        QMessageBox::warning(this,
                                   "Ошибка регистрации!",
                                   "Пароль слишком короткий!\nМинимальная длина: 6 символов.");
        ui->input_password->setFocus();
        ui->input_password->setSelection(0, password.length());
        return;
    }

    emit authorized(login, password);
}


void RegistrationForm::on_buttonBox_rejected()
{
    qDebug() << "Registration cancelled";
    close();
}

void RegistrationForm::on_to_authForm_clicked()
{
    qDebug() << "Switch to login requested";
    emit switchToLogin();
}
