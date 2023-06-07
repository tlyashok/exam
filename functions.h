#pragma once
#include <QString>
#include <QByteArray>
#include <QByteArray>

#include "singleton.h"
#include "database.h"
#include "mytcpserver.h"

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

class Functions : public Singleton<Functions>
{
    friend class Singleton<Functions>;
public:

    QByteArray parse(QString userMessage);

    QByteArray start(int sender, QString login, QString roomname);

    QByteArray break_user(int sender);

    QByteArray stats();

    QByteArray rooms();

    QByteArray newroom(QString name);

    bool check_room(QString roomname);

    QByteArray user_list(QString roomname);

};


#endif
// FUNCTIONS_H
