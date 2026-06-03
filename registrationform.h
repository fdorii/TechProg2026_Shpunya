#ifndef REGISTRATIONFORM_H
#define REGISTRATIONFORM_H

#include <QWidget>

namespace Ui {
class RegistrationForm;
}

class RegistrationForm : public QWidget
{
    Q_OBJECT
    friend class TestRegistrationForm;

public:
    explicit RegistrationForm(QWidget *parent = nullptr);
    ~RegistrationForm();

signals:
    void authorized(QString login, QString password);
    void switchToLogin();  // Сигнал переключения на авторизацию

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_to_authForm_clicked();

private:
    Ui::RegistrationForm *ui;
};

#endif // REGISTRATIONFORM_H
