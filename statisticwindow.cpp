#include "statisticwindow.h"
#include "ui_statisticwindow.h"
#include <QDebug>

StatisticForm::StatisticForm(const QString &login, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticForm),
    m_login(login)
{
    ui->setupUi(this);
    ui->usernameLabel->setText(m_login);
    qDebug() << "StatisticForm created for" << m_login;
}

StatisticForm::~StatisticForm()
{
    delete ui;
}

void StatisticForm::setStatistics(const QJsonObject &stats)
{
    qDebug() << "=== STEP 15: setStatistics entered ===";
    qDebug() << "=== STEP 16: stats object =" << stats;
    auto getInt = [&](const QString &task, const QString &field) {
        int val = stats[task].toObject()[field].toInt(0);
        qDebug() << "=== getInt:" << task << field << "=" << val;
        return val;
    };

    ui->cnt_1taskLabel->setText(QString::number(getInt("task1", "total")));
    qDebug() << "=== STEP 17: cnt_1taskLabel set to" << ui->cnt_1taskLabel->text();
    ui->r_1taskLabel->setText(QString::number(getInt("task1", "correct")));
    ui->wr_1taskLabel->setText(QString::number(getInt("task1", "wrong")));

    ui->cnt_2taskLabel->setText(QString::number(getInt("task2", "total")));
    ui->r_2taskLabel->setText(QString::number(getInt("task2", "correct")));
    ui->wr_2taskLabel->setText(QString::number(getInt("task2", "wrong")));

    ui->cnt_3taskLabel->setText(QString::number(getInt("task3", "total")));
    ui->r_3taskLabel->setText(QString::number(getInt("task3", "correct")));
    ui->wr_3taskLabel->setText(QString::number(getInt("task3", "wrong")));

    qDebug() << "=== STEP 18: All labels updated ===";
}
