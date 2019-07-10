#ifndef DB_H
#define DB_H
#include <QSqlQuery>

class DB
{
public:
    DB();
    DB(const QString dbname);
    QSqlQuery myQuery;

    void query(QString queryString);
    void prepare(const QString prepareString, const QStringList& args);
};

#endif // DB_H
