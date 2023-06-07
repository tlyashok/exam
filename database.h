#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

#include "singleton.h"

class DataBase : public Singleton<DataBase>
{
    friend class Singleton<DataBase>;
private:
    DataBase();
    DataBase(const DataBase& ) = delete;
    DataBase& operator = (DataBase &) = delete;
    ~DataBase();

public:
    QSqlDatabase db;
    QVector<QMap<QString, QString>> db_request(QString request);
    bool db_created();
    void create_db();
};

#endif // DATABASE_H
