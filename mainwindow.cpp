#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "singletonclient.h"
#include "task2form.h"
#include "task1.h"
#include "task3.h"
#include "statisticwindow.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(const QString &login, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_currentLogin(login)
{
    ui->setupUi(this);
    qDebug() << "MainWindow created for user:" << m_currentLogin;
}

MainWindow::~MainWindow()
{
    qDebug() << "MainWindow destroyed";
    if (taskForm) {
            taskForm->close();
            delete taskForm;
        }
    delete ui;
}

void MainWindow::appendServerMessage(QString msg)
{
    // Если есть текстовое поле для логов
    // ui->logTextEdit->append(msg);
    qDebug() << "Server message in MainWindow:" << msg;
}

void MainWindow::on_pushButton_3_clicked()
{
    qDebug() << "Opening Task2Form";

    if (!taskForm) {
        taskForm = new Task2Form(m_currentLogin, 2);
        taskForm->setAttribute(Qt::WA_DeleteOnClose);

        connect(taskForm, &Task2Form::destroyed, this, [this]() {
            taskForm = nullptr;
            qDebug() << "Task2Form closed";
        });
    }

    taskForm->generateNewTask();
    taskForm->show();
}

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

void MainWindow::updateConnectionStatus(bool connected)
{
    if (connected) {
        if (statusBar()) {
            statusBar()->showMessage("Connected to server");
        }
        qDebug() << "Status: Connected to server";
    } else {
        if (statusBar()) {
            statusBar()->showMessage("Disconnected from server");
        }
        qDebug() << "Status: Disconnected from server";
    }
}

void MainWindow::on_StatisticButton_clicked()
{
    qDebug() << "Static button clicked";
    if (!statisticForm) {
        statisticForm = new StatisticForm(m_currentLogin);
        connect(statisticForm, &StatisticForm::destroyed, this, [this]() {
            statisticForm = nullptr;
            qDebug() << "StatisticForm closed";
        });
    }

    statisticForm->show();
    statisticForm->raise();
    statisticForm->activateWindow();
}

