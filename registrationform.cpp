#include "registrationform.h"
#include "ui_registrationform.h"
#include <QDebug>

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
        return;
    }

    if (password.isEmpty()) {
        qDebug() << "Password is empty!";
        return;
    }

    if (password.length() < 6) {
        qDebug() << "Password too short!";
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
