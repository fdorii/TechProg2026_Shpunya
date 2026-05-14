#include "statisticwindow.h"
#include "ui_statisticwindow.h"
#include "singletonclient.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QDebug>

StatisticForm::StatisticForm(const QString &login, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticForm),
    m_currentLogin(login)
{
    ui->setupUi(this);
    ui->usernameLabel->setText(m_currentLogin);
    qDebug() << "StatisticForm created for" << login;
    connect(SingletonClient::getInstance(), &SingletonClient::msg_from_server,
            this, [this](const QString &msg) {
        qDebug() << "onServerMessage (lambda):" << msg;
        // ... код обновления лейблов ...
        QJsonDocument doc = QJsonDocument::fromJson(msg.toUtf8());
        if (!doc.isObject()) return;
        QJsonObject obj = doc.object();

        auto getInt = [&obj](const QString &taskKey, const QString &field) {
            return obj[taskKey].toObject()[field].toInt(0);
        };

        // Заполняем метки
        ui->cnt_1taskLabel->setText(QString::number(getInt("task1", "total")));
        ui->r_1taskLabel->setText(QString::number(getInt("task1", "correct")));
        ui->wr_1taskLabel->setText(QString::number(getInt("task1", "wrong")));

        ui->cnt_2taskLabel->setText(QString::number(getInt("task2", "total")));
        ui->r_2taskLabel->setText(QString::number(getInt("task2", "correct")));
        ui->wr_2taskLabel->setText(QString::number(getInt("task2", "wrong")));

        ui->cnt_3taskLabel->setText(QString::number(getInt("task3", "total")));
        ui->r_3taskLabel->setText(QString::number(getInt("task3", "correct")));
        ui->wr_3taskLabel->setText(QString::number(getInt("task3", "wrong")));
    });
    requestStats();
}

StatisticForm::~StatisticForm()
{
    qDebug() << "StatisticForm destroyed";
    delete ui;
}

void StatisticForm::requestStats()
{
    SingletonClient *client = SingletonClient::getInstance();
    if (!client->isConnected()) {
        qWarning() << "Not connected to server";
        return;
    }

    QJsonObject json;
    json["type"] = "get_detailed_stats";
    json["login"] = m_currentLogin;
    client->send_msg_to_server(
        QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Compact)));
}

void StatisticForm::onServerMessage(const QString &msg)
{
    qDebug() << "StatisticForm::onServerMessage called with:" << msg;
    QJsonDocument doc = QJsonDocument::fromJson(msg.toUtf8());
    if (!doc.isObject()) return;

    QJsonObject obj = doc.object();
    if (!obj.contains("task1")) return;   // это не ответ статистики

    // Вспомогательная функция чтения целого числа
    auto getInt = [&](const QString &taskKey, const QString &field) {
        return obj[taskKey].toObject()[field].toInt(0);
    };

    // Заполняем метки для трёх задач
    ui->cnt_1taskLabel->setText(QString::number(getInt("task1", "total")));
    ui->r_1taskLabel->setText(QString::number(getInt("task1", "correct")));
    ui->wr_1taskLabel->setText(QString::number(getInt("task1", "wrong")));

    ui->cnt_2taskLabel->setText(QString::number(getInt("task2", "total")));
    ui->r_2taskLabel->setText(QString::number(getInt("task2", "correct")));
    ui->wr_2taskLabel->setText(QString::number(getInt("task2", "wrong")));

    ui->cnt_3taskLabel->setText(QString::number(getInt("task3", "total")));
    ui->r_3taskLabel->setText(QString::number(getInt("task3", "correct")));
    ui->wr_3taskLabel->setText(QString::number(getInt("task3", "wrong")));

    qDebug() << "Stats updated";
}
