#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "singletonclient.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug() << "MainWindow created and shown";
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
