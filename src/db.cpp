#include "db.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include "mainwindow.h"
#include <QDebug>

DB::DB()
{
    QSqlDatabase database;
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        database = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(confDatabase);
    }
    if (!database.open()) {
        qDebug() << "Failed to open database while querying.";
    } else {
        this->myQuery = QSqlQuery(database);
    }
}

DB::DB(const QString dbname)
{
    QSqlDatabase database;
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        database = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(dbname);
    }
    if (!database.open()) {
        qDebug() << "Failed to open database while querying.";
    } else {
        this->myQuery = QSqlQuery(database);
    }
}

void DB::query(QString queryString)
{
    this->myQuery.prepare(queryString);
    if (!this->myQuery.exec()) {
        qDebug() << queryString << "exec fail!";
    }
}

void DB::prepare(const QString prepareString, const QStringList &args)
{
    this->myQuery.prepare(prepareString);
    for (int i = 0; i < args.length(); ++i) {
        this->myQuery.bindValue(i, args[i]);
    }
    if (!this->myQuery.exec()) {
        qDebug() << prepareString << args << "exec fail!";
    }
}
