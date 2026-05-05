#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "singletonclient.h"
#include "task1.h"
#include "task3.h"
#include "statisticwindow.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug() << "MainWindow created and shown";
    m_currentLogin = "sho"; // заглушка
}


MainWindow::~MainWindow()
{
    qDebug() << "MainWindow destroyed";
    delete ui;
}

void MainWindow::appendServerMessage(QString msg)
{
    // Если есть текстовое поле для логов
    // ui->logTextEdit->append(msg);
    qDebug() << "Server message in MainWindow:" << msg;
}

void MainWindow::on_exitButton_clicked()
{
    qDebug() << "Exit button clicked";
    close();
}

void MainWindow::on_logoutButton_clicked()
{
    qDebug() << "Logout button clicked";

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Выход",
        "Вы уверены, что хотите выйти?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        emit logout();
        close();
    }
}

// Обработчик для 1 задачи
void MainWindow::on_pushButton_2_clicked()
{
    qDebug() << "Task 1 button clicked";

    // окно
    Task1 *taskWindow = new Task1(m_currentLogin, 1);
    taskWindow->setAttribute(Qt::WA_DeleteOnClose);
    taskWindow->show();
}

// Обработчик для 3 задачи
void MainWindow::on_pushButton_4_clicked()
{
    qDebug() << "Task 3 button clicked";

    Task3 *taskWindow = new Task3(m_currentLogin, 3);
    taskWindow->setAttribute(Qt::WA_DeleteOnClose);
    taskWindow->show();
}

// Обработчик для кнопки "Показать статистику"
void MainWindow::on_pushButton_5_clicked()
{
    qDebug() << "Statistics button clicked";

    QMessageBox::information(this, "Статистика", "Окно статистики в разработке!");
}
