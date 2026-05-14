#include "database.h"
#include <QCoreApplication>

// Инициализация статических переменных
DataBase* DataBase::p_instance = nullptr;
DataBaseDestroyer DataBase::destroyer;

// Конструктор
DataBase::DataBase()
{
    // Подключаемся к базе
    db = QSqlDatabase::addDatabase("QSQLITE");
    // Путь задаётся через переменную окружения DB_PATH или по умолчанию /app/data/SQLite.db
    QString dbPath = QStringLiteral(PROJECT_ROOT) + "/SQLite.db";
    db.setDatabaseName(dbPath);
    qDebug() << "Using database file:" << db.databaseName();

    if (!db.open()) {
        qDebug() << "Database error:" << db.lastError().text();
    } else {
        qDebug() << "Database connected";
    }
}

// Деструктор
DataBase::~DataBase()
{
    if (db.isOpen()) {
        db.close();
        qDebug() << "Database closed";
    }
}

// Получение экземпляра синглтона
DataBase* DataBase::getInstance()
{
    if (!p_instance) {
        p_instance = new DataBase();
        destroyer.initialize(p_instance);
    }
    return p_instance;
}

// Авторизация
bool DataBase::auth(QString login, QString password)
{
    QSqlQuery query;
    query.prepare("SELECT login FROM User WHERE login = :login AND password = :password");
    query.bindValue(":login", login);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {
        qDebug() << "Auth:" << login;
        return true;
    }

    qDebug() << "Auth failed:" << login;
    return false;
}

// Регистрация
bool DataBase::reg(QString login, QString password)
{
    QSqlQuery query;
    query.prepare("INSERT INTO User (login, password) VALUES (:login, :password)");
    query.bindValue(":login", login);
    query.bindValue(":password", password);

    if (query.exec()) {
        qDebug() << "Reg:" << login;
        return true;
    }

    qDebug() << "Reg failed:" << query.lastError().text();
    return false;
}

// Получение статистики
int DataBase::getStats(QString login)
{
    QSqlQuery query;
    query.prepare(
        "SELECT COUNT(*) FROM Results "
        "WHERE login = :login AND solved = 1"
        );
    query.bindValue(":login", login);

    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        qDebug() << "Stats for" << login << ":" << count << "solved tasks";
        return count;
    }

    qDebug() << "Failed to get stats for" << login;
    return 0;
}

//Сохранение результата
bool DataBase::saveResult(QString login, int taskId, bool solved)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO Results (login, task_id, solved) "
        "VALUES (:login, :task_id, :solved)"
        );
    query.bindValue(":login", login);
    query.bindValue(":task_id", taskId);
    query.bindValue(":solved", solved);

    if (query.exec()) {
        qDebug() << "Saved result for" << login
                 << "- task" << taskId << (solved ? "correct" : "incorrect");
        return true;
    }

    qDebug() << "Failed to save result:" << query.lastError().text();
    return false;
}

QJsonObject DataBase::getDetailedStats(QString login)
{
    QJsonObject result;
    QSqlQuery query;
    query.prepare(
        "SELECT task_id, COUNT(*), SUM(solved) "
        "FROM Results WHERE login = :login "
        "GROUP BY task_id");
    query.bindValue(":login", login);

    if (query.exec()) {
        while (query.next()) {
            int taskId = query.value(0).toInt();
            int total = query.value(1).toInt();
            int correct = query.value(2).toInt();
            int wrong = total - correct;

            QJsonObject taskObj;
            taskObj["total"] = total;
            taskObj["correct"] = correct;
            taskObj["wrong"] = wrong;
            result[QString("task%1").arg(taskId)] = taskObj;
        }
    } else {
        qDebug() << "Failed to get detailed stats:" << query.lastError().text();
    }

    // Гарантируем, что для задач 1–3 всегда есть объекты (даже если записей нет)
    for (int i = 1; i <= 3; ++i) {
        QString key = QString("task%1").arg(i);
        if (!result.contains(key)) {
            QJsonObject emptyObj;
            emptyObj["total"] = 0;
            emptyObj["correct"] = 0;
            emptyObj["wrong"] = 0;
            result[key] = emptyObj;
        }
    }
    return result;
}
