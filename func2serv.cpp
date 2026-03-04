#include "func2serv.h"
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
    else {
            return QByteArray("error");
    }
}

// Авторизация
QString auth(QString login, QString password)
{
    qDebug() << "Auth:" << login << password;
    //dataBase::
    
    if (login == "user" && password == "123") {
        return "auth+\n";
    }
    else {
        return "auth-\n";
    }
}

// Регистрация
QString reg(QString login, QString password)
{
    qDebug() << "Reg:" << login << password;
    //dataBase::
    
    return "reg+\n"; 
}


