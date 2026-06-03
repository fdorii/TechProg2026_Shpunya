#ifndef TASK2FORM_H
#define TASK2FORM_H

#include <QWidget>
#include <QRandomGenerator>

namespace Ui {
class Task2Form;
}

class Task2Form : public QWidget
{
    Q_OBJECT
    friend class TestTask2Form;

public:
    explicit Task2Form(QWidget *parent = nullptr);
    explicit Task2Form(const QString &login, int taskId, QWidget *parent = nullptr); // <-- новые параметры
    ~Task2Form();
    void generateNewTask();

private slots:
    void on_checkButton_clicked();

private:
    Ui::Task2Form *ui;
    double m_a;
    double m_b;
    double m_fa;
    double m_fb;
    double m_correctAnswer;

    void sendResult(bool solved);
    QString m_login;
    int m_taskId;

    double calculateIntegral(double a, double b, double fa, double fb);
    void generateRandomTask();
    void updateTaskDisplay();

    // Допустимая погрешность для проверки ответа
    static constexpr double EPSILON = 0.001;
};

#endif // TASK2FORM_H
