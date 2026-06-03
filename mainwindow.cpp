#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "singletonclient.h"
#include "task2form.h"
#include "task1.h"
#include "task3.h"
#include "statisticwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>


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
    qDebug() << "=== STEP 6: appendServerMessage called, msg =" << msg;
    qDebug() << "=== STEP 7: m_waitingForStats =" << m_waitingForStats;
    if (m_waitingForStats) {
        qDebug() << "=== STEP 8: Waiting flag is TRUE, parsing... ===";
        QJsonDocument doc = QJsonDocument::fromJson(msg.toUtf8());
        qDebug() << "=== STEP 9: JSON parsed, isObject =" << doc.isObject();
        if (doc.isObject()) {
                    QJsonObject obj = doc.object();
                    qDebug() << "=== STEP 10: JSON object keys =" << obj.keys();
                    qDebug() << "=== STEP 11: Contains 'task1' =" << obj.contains("task1");

                    if (obj.contains("task1")) {
                        qDebug() << "=== STEP 12: All checks passed, delivering to StatisticForm ===";
                        m_waitingForStats = false;
                        if (statisticForm) {
                            statisticForm->setStatistics(obj);
                            qDebug() << "=== STEP 13: setStatistics called ===";
                        } else {
                            qDebug() << "=== ERROR: statisticForm is NULL! ===";
                        }
                        return;
        }
    }
    qDebug() << "=== STEP 14: Not a stats message, ignoring ===";
}
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
    qDebug() << "=== STEP 1: StatisticButton clicked ===";
    if (!statisticForm) {
        qDebug() << "=== STEP 2: Creating new StatisticForm ===";
        statisticForm = new StatisticForm(m_currentLogin);
        statisticForm->setAttribute(Qt::WA_DeleteOnClose);
        connect(statisticForm, &QObject::destroyed, this, [this]() {
            statisticForm = nullptr;
        });
    }
    statisticForm->show();
    statisticForm->raise();
    statisticForm->activateWindow();

    // Устанавливаем флаг, что ждём статистику
    m_waitingForStats = true;
    qDebug() << "=== STEP 3: Flag set, sending request ===";

    // Отправляем запрос
    QJsonObject json;
    json["type"] = "get_detailed_stats";
    json["login"] = m_currentLogin;
    QString request = QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Compact));
    qDebug() << "=== STEP 4: Request string =" << request;
    SingletonClient::getInstance()->send_msg_to_server(request);
    qDebug() << "Stats requested";
}
