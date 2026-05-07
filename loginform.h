#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>

namespace Ui {
class LogInForm;
}

class LogInForm : public QWidget
{
    Q_OBJECT

public:
    explicit LogInForm(QWidget *parent = nullptr);
    ~LogInForm();

    void clearLoginField();
    void clearPasswordField();
    void clearAllFields();

signals:
    void loginAttempt(QString login, QString password);
    void switchToRegistration();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_to_regButton_clicked();

private:
    Ui::LogInForm *ui;

    bool validateInput(QString login, QString password);
};

#endif // LOGINFORM_H
