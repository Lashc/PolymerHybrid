#ifndef QUERY_H
#define QUERY_H

class QSqlError;
class QStringList;
class QString;

// Functions to produce and execute SQL queries for 'polymer_hybrid.db'
namespace polyDB {
    QSqlError insertInto(const int dataID, QStringList fields, QStringList data);
    QSqlError update(const int dataID, QStringList fields, QStringList data, const int rowID);
}

#endif // QUERY_H
