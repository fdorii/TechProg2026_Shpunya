#ifndef TASK1_H
#define TASK1_H

#include <QWidget>
#include "ui_task1.h"

namespace Ui {
class Task1;
}

class Task1 : public QWidget
{
    Q_OBJECT

public:
    explicit Task1(QString login, int taskId, QWidget *parent = nullptr);
    ~Task1();

private slots:
    void on_checkButton_clicked();

private:
    void generateRandomTask();

    Ui::Task1 *ui;

    QString m_login;
    int m_taskId;
    double m_a, m_b;        // параметры функции
    double m_x0;            // начальное приближение
    double m_correctAnswer; // правильный ответ
};

#endif // TASK1_H
