#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QJsonObject>
#include <QString>
#include <QDebug>

// вспомогательный класс для удаления синглтона
class DataBaseDestroyer
{
private:
    class DataBase * p_instance;
public:
    ~DataBaseDestroyer() { delete p_instance; }
    void initialize(DataBase * p) { p_instance = p; }
};

// Основной класс базы данных (синглтон)
class DataBase
{
private:
    static DataBase * p_instance;
    static DataBaseDestroyer destroyer;

protected:
    DataBase();
    DataBase(const DataBase&);
    DataBase& operator=(DataBase&);
    ~DataBase();

    friend class DataBaseDestroyer;

    // Объект подключения к БД
    QSqlDatabase db;


public:
    static DataBase* getInstance();

    // Методы для работы с данными
    bool auth(QString login, QString password);  // авторизация
    bool reg(QString login, QString password);  // регистрация
    int  getStats(QString login);  // статистика пользователя
    bool saveResult(QString login, int taskId, bool solved);
    QJsonObject getDetailedStats(QString login);

};


#endif // DATABASE_H
