#include "loginform.h"
#include "ui_loginform.h"
#include <QDebug>

LogInForm::LogInForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogInForm)
{
    ui->setupUi(this);
    qDebug() << "LogInForm created";
}

LogInForm::~LogInForm()
{
    qDebug() << "LogInForm destroyed";
    delete ui;
}

void LogInForm::on_buttonBox_accepted()
{
    QString login = ui->input_login->text().trimmed();
    QString password = ui->input_password->text();

    qDebug() << "Login attempt - Login:" << login;

    if (!login.isEmpty() && !password.isEmpty()) {
        emit loginAttempt(login, password);
    } else {
        qDebug() << "Login or password is empty!";
    }
}

void LogInForm::on_buttonBox_rejected()
{
    qDebug() << "Login cancelled";
    close();
}

void LogInForm::on_to_regButton_clicked()
{
    qDebug() << "Switch to registration requested";
    emit switchToRegistration();
}
