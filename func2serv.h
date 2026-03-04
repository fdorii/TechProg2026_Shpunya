#ifndef FUNC2SERV_H
#define FUNC2SERV_H

#include <QByteArray>
#include <QString>

// Основная функция парсинга
QByteArray parsing(QString data_from_client);

// Функции-обработчики
QString auth(QString login, QString password);
QString reg(QString login, QString password);

#endif // FUNC2SERV_H