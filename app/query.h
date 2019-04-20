#ifndef QUERY_H
#define QUERY_H

class QSqlError;
class QStringList;
class QString;

// Functions to produce and execute SQL queries for 'polymer_hybrid.db'
namespace polyDB {
    QSqlError createTables();
    QSqlError insertEntry(const int dataID, QStringList fields, QStringList data);
    QSqlError updateEntry(const int dataID, QStringList fields, QStringList data, const int rowID);
    QSqlError deleteEntry(QString table, const int rowID);
}

#endif // QUERY_H
