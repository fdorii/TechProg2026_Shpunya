#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class  StatisticForm;
class Task2Form;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString &login, QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void appendServerMessage(QString msg);
    void updateConnectionStatus(bool connected);

signals:
    void logout();

private slots:
    void on_exitButton_clicked();
    void on_logoutButton_clicked();
    void on_pushButton_3_clicked(); // 2 задача
    void on_pushButton_2_clicked(); // 1 задача
    void on_pushButton_4_clicked(); // 3 задача

    void on_StatisticButton_clicked();

private:
    Ui::MainWindow *ui;
    Task2Form *taskForm = nullptr;
    StatisticForm *statisticForm = nullptr;
    QString m_currentLogin;
};

#endif // MAINWINDOW_H
