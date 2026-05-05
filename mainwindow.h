#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void appendServerMessage(QString msg);

signals:
    void logout();

private slots:
    void on_exitButton_clicked();
    void on_logoutButton_clicked();
    void on_pushButton_2_clicked();  // 1 задача
    void on_pushButton_5_clicked();  // Показать статистику
private:
    Ui::MainWindow *ui;
    QString m_currentLogin;
};

#endif // MAINWINDOW_H
