#include "func2serv.h"
#include "database.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

QByteArray parsing(QString data_from_client)
{
    // Удаляем завершающий \x01, если он есть
    if (data_from_client.endsWith('\x01'))
        data_from_client.chop(1);
    qDebug() << "Parsing:" << data_from_client;

    // Пробуем распарсить как JSON
    QJsonDocument doc = QJsonDocument::fromJson(data_from_client.toUtf8());

    if (doc.isObject()) {
        QJsonObject json = doc.object();
        QString type = json["type"].toString();
        QString login = json["login"].toString();
        QString password = json["password"].toString();

        qDebug() << "JSON parsed - Type:" << type << "Login:" << login;

        if (type == "login" || type == "auth") {
            return auth(login, password).toUtf8();
        }
        else if (type == "register" || type == "reg") {
            return reg(login, password).toUtf8();
        }
        else if (type == "get_stats") {
            return get_stats(login).toUtf8();
        }
        else if (type == "get_tasks") {
            return get_tasks(login).toUtf8();
        }
        else if (type == "get_task") {
            QString task_id = json["task_id"].toString();
            return get_task(login, task_id).toUtf8();
        }
        else if (type == "check_answer") {
            QString task_id = json["task_id"].toString();
            QString answer = json["answer"].toString();
            return check_answer(login, task_id, answer).toUtf8();
        }
        else if (type == "get_detailed_stats") {
                    QJsonObject stats = DataBase::getInstance()->getDetailedStats(login);
                    return QJsonDocument(stats).toJson(QJsonDocument::Compact);
                }
    }
     
    QStringList data_from_client_list = data_from_client.split(QLatin1Char(' '));
    
    if (data_from_client_list.isEmpty()) {
        return QByteArray("error: empty request\n");
    }
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

    else if (nameOfFunc == "save_result") {
        if (data_from_client_list.size() < 3) return QByteArray("error: invalid params\n");
        QString login = data_from_client_list.at(0);
        int taskId = data_from_client_list.at(1).toInt();
        bool solved = (data_from_client_list.at(2).toInt() == 1);
        DataBase::getInstance()->saveResult(login, taskId, solved);
        return QByteArray("result_saved\n");
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


