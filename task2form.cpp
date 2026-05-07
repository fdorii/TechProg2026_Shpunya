#include "task2form.h"
#include "ui_task2form.h"
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <cmath>

Task2Form::Task2Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Task2Form)
{
    ui->setupUi(this);

    ui->answerLineEdit->setPlaceholderText("Введите ответ (число)");
    ui->answerLineEdit->setClearButtonEnabled(true);

    ui->resultLabel->clear();
    ui->resultLabel->setAlignment(Qt::AlignCenter);
    ui->resultLabel->setWordWrap(true);

    ui->answerLineEdit->setStyleSheet("");

    generateNewTask();

    qDebug() << "Task2Form created";
}

Task2Form::~Task2Form()
{
    qDebug() << "Task2Form destroyed";
    delete ui;
}

void Task2Form::generateNewTask()
{
    generateRandomTask();
    updateTaskDisplay();

    ui->answerLineEdit->clear();
    ui->resultLabel->clear();
    ui->answerLineEdit->setStyleSheet("");

    ui->answerLineEdit->setFocus();
}

void Task2Form::generateRandomTask()
{
    QRandomGenerator *rng = QRandomGenerator::global();

    // Границы отрезка [a, b]
    m_a = static_cast<double>(rng->bounded(-100, 101)) / 10.0;  // от -10.0 до 10.0 с шагом 0.1

    do {
        m_b = static_cast<double>(rng->bounded(-100, 101)) / 10.0;
    } while (std::abs(m_b - m_a) < 1.0);  // Разница между a и b не менее 1.0

    // Значения функции на концах отрезка
    m_fa = static_cast<double>(rng->bounded(-50, 51)) / 10.0;
    m_fb = static_cast<double>(rng->bounded(-50, 51)) / 10.0;

    // Вычисляем интеграл методом трапеций при n=1
    m_correctAnswer = calculateIntegral(m_a, m_b, m_fa, m_fb);

    qDebug() << "Generated task: a=" << m_a << "b=" << m_b
             << "f(a)=" << m_fa << "f(b)=" << m_fb
             << "answer=" << m_correctAnswer;
}

double Task2Form::calculateIntegral(double a, double b, double fa, double fb)
{
    double h = b - a;
    double result = h * (fa + fb) / 2.0;

    result = std::round(result * 1000.0) / 1000.0;

    return result;
}

void Task2Form::updateTaskDisplay()
{
    QString condition = QString(
        "Дано:\n"
        "• Отрезок [a, b] = [%1, %2]\n"
        "• Значения функции на концах отрезка:\n"
        "  f(%1) = f(a) = %3\n"
        "  f(%2) = f(b) = %4\n\n"
        "Найти:\n"
        "Приближённое значение определённого интеграла\n"
        "∫[%1,%2] f(x) dx методом трапеций при n = 1.\n\n"
        "Формула для вычисления:\n"
        "I ≈ (b - a) × (f(a) + f(b)) ÷ 2\n\n"
        "Ответ округлите до 3 знаков после запятой."
    ).arg(m_a)
     .arg(m_b)
     .arg(m_fa)
     .arg(m_fb);

    ui->conditionLabel->setText(condition);
    ui->conditionLabel->setWordWrap(true);
}

void Task2Form::on_checkButton_clicked()
{
    QString userInput = ui->answerLineEdit->text().trimmed();

    // Проверка на пустой ввод
    if (userInput.isEmpty()) {
        QMessageBox::warning(this,
                           "Проверка",
                           "Введите ответ перед проверкой!");
        ui->answerLineEdit->setFocus();
        return;
    }

    // Проверка на корректность числа
    bool ok = false;
    double userAnswer = userInput.toDouble(&ok);

    if (!ok) {
        ui->resultLabel->setText("Ошибка: введите корректное число!");
        ui->resultLabel->setStyleSheet("color: red; font-weight: bold;");
        ui->answerLineEdit->setStyleSheet("border: 2px solid red;");
        ui->answerLineEdit->setFocus();
        ui->answerLineEdit->selectAll();
        return;
    }

    // Сравниваем с правильным ответом
    if (std::abs(userAnswer - m_correctAnswer) < EPSILON) {
        ui->resultLabel->setText(QString("Правильно!\n\nИнтеграл ≈ %1").arg(m_correctAnswer, 0, 'f', 3));
        ui->resultLabel->setStyleSheet("color: green; font-weight: bold; font-size: 14px;");
        ui->answerLineEdit->setStyleSheet("border: 2px solid green;");

        QMessageBox::information(this,
                                "Отлично!",
                                QString("Верно!\n\n"
                                       "Интеграл методом трапеций при n=1:\n"
                                       "I = (b - a) × (f(a) + f(b)) ÷ 2\n"
                                       "I = (%1 - %2) × (%3 + %4) ÷ 2\n"
                                       "I = %5 × %6 ÷ 2\n"
                                       "I = %7")
                                .arg(m_b)
                                .arg(m_a)
                                .arg(m_fa)
                                .arg(m_fb)
                                .arg(m_b - m_a)
                                .arg(m_fa + m_fb)
                                .arg(m_correctAnswer, 0, 'f', 3));

        // Генерируем новую задачу через небольшое время
        QTimer::singleShot(500, this, &Task2Form::generateNewTask);

    } else {
        // Неправильный ответ
        QString hint;
        double difference = userAnswer - m_correctAnswer;

        if (std::abs(difference) > 10.0) {
            hint = "Подсказка: проверьте формулу трапеций:\n"
                   "I = h × (f(a) + f(b)) ÷ 2, где h = b - a";
        } else if (std::abs(difference) > 1.0) {
            hint = "Подсказка: проверьте вычисление h = b - a";
        } else {
            hint = "Подсказка: вы близко! Проверьте арифметику.";
        }

        ui->resultLabel->setText(QString("Неправильно\n\n%1").arg(hint));
        ui->resultLabel->setStyleSheet("color: red; font-weight: bold;");
        ui->answerLineEdit->setStyleSheet("border: 2px solid red;");
        ui->answerLineEdit->setFocus();
        ui->answerLineEdit->selectAll();

        qDebug() << "Wrong answer: user=" << userAnswer << "correct=" << m_correctAnswer;
    }
}
