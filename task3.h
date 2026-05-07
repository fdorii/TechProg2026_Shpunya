#ifndef TASK3_H
#define TASK3_H

#include <QWidget>
#include <QString>
#include <vector>
#include <map>

QT_BEGIN_NAMESPACE
namespace Ui {
class Task3;
}
QT_END_NAMESPACE

class Task3 : public QWidget
{
    Q_OBJECT

public:
    explicit Task3(QString login, int taskId, QWidget *parent = nullptr);
    ~Task3();

private slots:
    void on_checkButton_clicked();

private:
    void generateRandomTask();

    Ui::Task3 *ui;

    QString m_login;
    int m_taskId;

    // данные задачи
    std::map<int, std::vector<int>> tree;
    int root;

    QString correctAnswer;
};

#endif // TASK3_H