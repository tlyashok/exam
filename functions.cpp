#include <QString>
#include <QStringList>
#include <QByteArray>
#include "functions.h"

QByteArray Functions::parse(QString userMessage) {
    QStringList data = userMessage.split('&');
    if (data[1] == "start") {
        return start(data[0].toInt(), data[2], data[3]);
    } else if (data[1] == "newroom") {
        return newroom(data[2]);
    } else if (data[1] == "break//") {
        return break_user(data[0].toInt());
    } else if (data[1] == "rooms//") {
        return rooms();
    } else if (data[1] == "stats//") {
        return stats();
    }
}

QByteArray Functions::start(int sender, QString login, QString roomname)
{
    QVector<QMap<QString, QString>> answer =
            DataBase::getInstance()->db_request(
                QString("SELECT * FROM rooms WHERE user_id = %1 or login = '%2'").arg(QString::number(sender), login));
    if (answer.size() != 0) {
        return QByteArray(QString("Вы уже авторизированы").toUtf8());
    }
    answer = DataBase::getInstance()->db_request(
                QString("SELECT * FROM rooms_name WHERE roomname = '%1'").arg(roomname));
    if (answer.size() != 0) {
        DataBase::getInstance()->db_request(QString("INSERT INTO rooms(user_id, login, roomname) VALUES(%1, '%2', '%3')").arg(QString::number(sender), login, roomname));
        if (check_room(roomname)) {
            return QByteArray(QString("").toUtf8());
        } else {
            return QByteArray(QString("Пользователь добавлен").toUtf8());
        }
    } else {
        return QByteArray(QString("Данной комнаты не существует").toUtf8());
    }

}

QByteArray Functions::break_user(int sender)
{
    QVector<QMap<QString, QString>> answer =
            DataBase::getInstance()->db_request(
                QString("SELECT * FROM rooms WHERE user_id = %1").arg(sender));
    if (answer.size() == 0) {
        return QByteArray(QString("Вы не авторизированы!").toUtf8());
    }
    DataBase::getInstance()->db_request(
                    QString("DELETE FROM rooms WHERE user_id = %1").arg(sender));
    return QByteArray(QString("Успешно!").toUtf8());
}

QByteArray Functions::stats()
{
    QVector<QMap<QString, QString>> answer =
            DataBase::getInstance()->db_request(
                QString("SELECT * FROM rooms"));
    QStringList rooms;
    QVector<QMap<QString, QString>> answer1 = DataBase::getInstance()->db_request(
                QString("SELECT * FROM rooms_name"));
    for (int i = 0; i < answer1.size(); i++) {
        rooms.append(answer1[i]["roomname"]);
    }
    QString ansForClient;
    for (int i = 0; i < rooms.size(); i++) {
        ansForClient += rooms[i] + QString(": ");
        for (int j = 0; j < answer.size(); j++) {
            qDebug() << answer[j]["roomname"];
            if (answer[j]["roomname"] == rooms[i]) {
                ansForClient += answer[j]["login"] + " ";
            }
        }
        ansForClient += QString("|");
    }
    return QByteArray(ansForClient.toUtf8());

}

QByteArray Functions::rooms()
{
    QVector<QMap<QString, QString>> answer =
            DataBase::getInstance()->db_request(QString("SELECT * FROM rooms_name"));
    QString ansForClient;
    for (int i = 0; i < answer.size(); i++) {
        ansForClient += answer[i]["roomname"] + QString(" ");
    }
    return QByteArray(ansForClient.toUtf8());
}

QByteArray Functions::newroom(QString name)
{
    QVector<QMap<QString, QString>> answer = DataBase::getInstance()->db_request(
                QString("SELECT * FROM rooms_name WHERE roomname = '%1'").arg(name));
    if (answer.size() == 0) {
        DataBase::getInstance()->db_request(
                    QString("INSERT INTO rooms_name VALUES('%1')").arg(name));
        return QByteArray(QString("Комната создана").toUtf8());
    }
    else {
        return QByteArray(QString("Комната уже существует").toUtf8());
    }
}

bool Functions::check_room(QString roomname)
{
    QVector<QMap<QString, QString>> answer = DataBase::getInstance()->db_request(
                QString("SELECT * FROM rooms WHERE roomname = '%1'").arg(roomname));
    if (answer.size()>=2) {
        QVector<int> users;
        for (int i = 0; i < answer.size(); i++) {
            users.append(answer[i]["user_id"].toInt());
        }
        QByteArray forSend = user_list(roomname);
        for (int i = 0; i < users.size(); i++) {
            if (MyTcpServer::getInstance()->mTcpSocket.contains(users[i])) {
                MyTcpServer::getInstance()->mTcpSocket[users[i]]->write(forSend);
            }
            break_user(users[i]);
        }
        return true;
    } else {
        return false;
    }
}

QByteArray Functions::user_list(QString roomname)
{
    QVector<QMap<QString, QString>> answer = DataBase::getInstance()->db_request(
                QString("SELECT * FROM rooms WHERE roomname = '%1'").arg(roomname));
    QStringList users;
    for (int i =0; i < answer.size(); i++) {
        users.append(answer[i]["login"]);
    }
    QString ansForClient;
    for (int i = 0; i < users.size();i++) {
        ansForClient += users[i] + " ";
    }
    return QByteArray(ansForClient.toUtf8());
}
