#include "task1.h"
#include "ui_task1.h"
#include "singletonclient.h"
#include <QRandomGenerator>
#include <QDebug>
#include <cmath>
#include <QMessageBox>

Task1::Task1(QString login, int taskId, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Task1)
    , m_login(login)
    , m_taskId(taskId)
{
    ui->setupUi(this);

    // окно
    setWindowTitle("Задача 1 - Метод Ньютона");
    setWindowFlags(Qt::Window);
    setAttribute(Qt::WA_DeleteOnClose);

    // фон
    setStyleSheet("background-color: rgb(240, 240, 240);");


    generateRandomTask();

    // Подключаем кнопку
    connect(ui->pushButton, &QPushButton::clicked,
            this, &Task1::on_checkButton_clicked);
}

Task1::~Task1()
{
    delete ui;
}

void Task1::generateRandomTask()
{
    QRandomGenerator *gen = QRandomGenerator::global();

    // Выбираем случайную функцию
    int funcType = gen->bounded(1, 5);  // 1-4

    if (funcType == 1) {
        // f(x) = x² - a
        m_a = gen->bounded(2, 10);
        m_x0 = gen->bounded(1, 5);
        m_correctAnswer = (m_x0 - (m_x0 * m_x0 - m_a) / (2 * m_x0));

        ui->label->setText(QString(
                               "Функция: f(x) = x² - %1\n"
                               "Производная: f'(x) = 2x\n"
                               "Начальное приближение: x₀ = %2\n\n"
                               "Найдите x₁ (первое приближение) методом Ньютона.\n"
                               "Округлите до 2 знаков после запятой.")
                               .arg(m_a).arg(m_x0));
    }
    else if (funcType == 2) {
        // f(x) = x² - a*x
        m_a = gen->bounded(2, 10);
        m_b = gen->bounded(1, 5);
        m_x0 = gen->bounded(1, 5);
        m_correctAnswer = (m_x0 - (m_x0 * m_x0 - m_a * m_x0) / (2 * m_x0 - m_a));

        ui->label->setText(QString(
                               "Функция: f(x) = x² - %1x\n"
                               "Производная: f'(x) = 2x - %1\n"
                               "Начальное приближение: x₀ = %2\n\n"
                               "Найдите x₁ (первое приближение) методом Ньютона.\n"
                               "Округлите до 2 знаков после запятой.")
                               .arg(m_a).arg(m_x0));
    }
    else if (funcType == 3) {
        // f(x) = x³ - a
        m_a = gen->bounded(2, 20);
        m_x0 = gen->bounded(2, 5);
        m_correctAnswer = (m_x0 - (m_x0 * m_x0 * m_x0 - m_a) / (3 * m_x0 * m_x0));

        ui->label->setText(QString(
                               "Функция: f(x) = x³ - %1\n"
                               "Производная: f'(x) = 3x²\n"
                               "Начальное приближение: x₀ = %2\n\n"
                               "Найдите x₁ (первое приближение) методом Ньютона.\n"
                               "Округлите до 2 знаков после запятой.")
                               .arg(m_a).arg(m_x0));
    }
    else if (funcType == 4) {
        // f(x) = sin(x) - a
        m_a = gen->bounded(1, 5) / 10.0;
        m_x0 = gen->bounded(0, 2);
        m_correctAnswer = (m_x0 - (sin(m_x0) - m_a) / cos(m_x0));

        ui->label->setText(QString(
                               "Функция: f(x) = sin(x) - %1\n"
                               "Производная: f'(x) = cos(x)\n"
                               "Начальное приближение: x₀ = %2\n\n"
                               "Найдите x₁ (первое приближение) методом Ньютона.\n"
                               "Округлите до 3 знаков после запятой.")
                               .arg(m_a).arg(m_x0));
    }
}

void Task1::on_checkButton_clicked()
{
    // Получаем ответ из поля ввода
    QString answerText = ui->lineEdit->text().trimmed();
    bool ok;
    double userAnswer = answerText.toDouble(&ok);

    // Очищаем поле ввода
    ui->lineEdit->clear();

    // Проверка: введено ли число
    if (!ok) {
        QMessageBox::warning(this, "эй!", "нужно число");
        return;
    }

    // Сравниваем ответ пользователя с правильным
    double delta = qAbs(userAnswer - m_correctAnswer);

    if (delta < 0.01) {  // погрешность 0.01
        QMessageBox::information(this, "результат", "ура молодец!!");
    } else {
        QMessageBox::information(this, "результат", "неправильно :(\n\nпопробуй еще");
    }
}
