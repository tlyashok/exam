#include "database.h"

bool DataBase::db_created()
{
    return this->db.tables() == QStringList{"rooms, rooms_name"};
}

void DataBase::create_db()
{
    QSqlQuery query(this->db);
    query.exec("CREATE TABLE rooms("
           "user_id INT8 PRIMARY KEY, "
           "roomname VARCHAR(64), "
           "login VARCHAR(64) NOT NULL"
           ")");
    query.exec("CREATE TABLE rooms_name("
           "roomname VARCHAR(64)"
           ")");
}

DataBase::DataBase()
{
    // Инициализируем базу данных из файла database.sqlite
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db.setDatabaseName("database.sqlite");

    // Выводим ошибки, если они есть, в консоль
    if (!(this->db.open()))
        qDebug()<<db.lastError().text();

    // Если нет таблиц Users, Tasks, UserGroups, то создаем их
    if (!this->db_created())
        this->create_db();
}

DataBase::~DataBase()
{
    // Закрываем базу данных
    db.close();
}

QVector<QMap<QString, QString>> DataBase::db_request(QString request)
{
    QSqlQuery query(this->db);
    query.exec(request);            // исполняем запрос

    QVector<QMap<QString, QString>> result; // таблица с запрашиваемыми данными

    if (query.lastError().isValid()) {
        qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
        return result;
    }

    QSqlRecord record = query.record();
    QVector<QString> columns;       // хранит столбцы возвращаемой таблицы

    for (int i = 0; i < record.count(); i++) {
        QString columnName = record.fieldName(i);
        columns.append(columnName);
    }

    while (query.next()) {
        QMap<QString, QString> row;
        for (int i = 0; i < columns.count(); i++) {
            // строки таблицы заполняются, каждая строка - хэшмап, ключ - название столбца, значение - то, что в ячейке
            QString columnName = query.record().fieldName(i);
            QString value = query.value(i).toString();
            row.insert(columnName, value);
        }
        result.append(row);
    }
    return result;
}

