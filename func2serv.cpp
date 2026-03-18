#include "func2serv.h"
#include "database.h"
#include <QDebug>

QByteArray parsing(QString data_from_client)
{
    qDebug() << "Parsing:" << data_from_client;
     
    QStringList data_from_client_list = data_from_client.split(QLatin1Char(' '));
    
    QString nameOfFunc = data_from_client_list.front();
    data_from_client_list.pop_front(); 
    
    // Вызываем нужную функцию в зависимости от имени
    if (nameOfFunc == "auth") {  
        return auth(data_from_client_list.at(0), 
                       data_from_client_list.at(1)).toUtf8();
    }
    else if (nameOfFunc == "reg") {
            return reg(data_from_client_list.at(0), 
                      data_from_client_list.at(1)).toUtf8();
    }
    else if (nameOfFunc == "get_stats") {
            return get_stats(data_from_client_list.at(0)).toUtf8();

    }
    else if (nameOfFunc == "get_tasks") {
            return get_tasks(data_from_client_list.at(0)).toUtf8();
        }

    else if (nameOfFunc == "get_task") {
            return get_task(data_from_client_list.at(0), data_from_client_list.at(1)).toUtf8();
        }

    else if (nameOfFunc == "check_answer") {
            return check_answer(data_from_client_list.at(0), data_from_client_list.at(1), data_from_client_list.at(2)).toUtf8();
        }

    else {
            return QByteArray("error\n");
    }
}

// Авторизация
QString auth(QString login, QString password)
{
    qDebug() << "Auth:" << login << password;

    if (DataBase::getInstance()->auth(login, password)) {
        return "auth+\n";
    }

    return "auth-\n";
}

// Регистрация
QString reg(QString login, QString password)
{
    qDebug() << "Reg:" << login << password;

    if (DataBase::getInstance()->reg(login, password)) {
        return "reg+\n";
    }

    return "reg-: registration failed\n";
}

QString get_stats(QString login)
{
    qDebug() << "GET_STATS for user:" << login;

    int count = DataBase::getInstance()->getStats(login);
    return QString("stats: %1\n").arg(count);
}

// Список задач для пользователя
QString get_tasks(QString login)
{
    qDebug() << "TASKS for user:" << login;
    return "tasks: 1,2,3\n";
}

// Условие конкретной задачи
QString get_task(QString login, QString task_id)
{
    qDebug() << "TASK - user:" << login << "task:" << task_id;

    if (task_id == "1") {
        return "task: Задача 1\n";
    }
    else if (task_id == "2") {
        return "task: Задача 2\n";
    }
    else if (task_id == "3") {
        return "task: Задача 3\n";
    }
    return "task: Условие задачи " + task_id + "\n";
}

// Проверка ответа
QString check_answer(QString login, QString task_id, QString answer)
{
    qDebug() << "CHECK_ANSWER - user:" << login
             << "task:" << task_id << "answer:" << answer;

    QString cleanAnswer = answer.trimmed();
    bool isCorrect = false;

    if (task_id == "1" && cleanAnswer == "3") {
        isCorrect = true;
    }
    else if (task_id == "2" && cleanAnswer == "5") {
        isCorrect = true;
    }
    else if (task_id == "3" && cleanAnswer == "11") {
        isCorrect = true;
    }

    DataBase::getInstance()->saveResult(login, task_id.toInt(), isCorrect);

    if (isCorrect) {
        return "correct\n";
    } else {
        return "incorrect\n";
    }
}


