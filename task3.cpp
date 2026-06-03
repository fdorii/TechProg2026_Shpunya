#include "task3.h"
#include "ui_task3.h"

#include <QRandomGenerator>
#include <QMessageBox>

Task3::Task3(QString login, int taskId, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Task3)
    , m_login(login)
    , m_taskId(taskId)
{
    ui->setupUi(this);

    setWindowTitle("Задача 3");
    setAttribute(Qt::WA_DeleteOnClose);

    generateRandomTask();

    connect(ui->pushButton, &QPushButton::clicked,
            this, &Task3::on_checkButton_clicked);
}

Task3::~Task3()
{
    delete ui;
}

void Task3::generateRandomTask()
{
    tree.clear();

    QRandomGenerator *gen = QRandomGenerator::global();

    int n = gen->bounded(5, 9);
    root = 1;

    for (int i = 2; i <= n; i++) {
        int parent = gen->bounded(1, i);
        tree[parent].push_back(i);
    }

    std::vector<int> leaves;
    for (int i = 1; i <= n; i++) {
        if (tree.find(i) == tree.end()) {
            leaves.push_back(i);
        }
    }

    correctAnswer = "";
    for (int v : leaves) {
        correctAnswer += QString::number(v) + " ";
    }
    correctAnswer = correctAnswer.trimmed();

    QString treeText = "Дерево (родитель -> дети):\n";
    for (auto &p : tree) {
        treeText += QString::number(p.first) + " -> ";
        for (int child : p.second) {
            treeText += QString::number(child) + " ";
        }
        treeText += "\n";
    }

    ui->label->setText(treeText + "\nНайдите листья (через пробел)");
}

void Task3::on_checkButton_clicked()
{
    QString user = ui->lineEdit->text().trimmed();
    ui->lineEdit->clear();

    if (user == correctAnswer) {
        QMessageBox::information(this, "Результат", "Верно!");
    } else {
        QMessageBox::information(this, "Результат",
            "Неверно\nПравильный ответ: " + correctAnswer);
    }
}