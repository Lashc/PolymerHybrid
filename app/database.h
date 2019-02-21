#ifndef DATABASE_H
#define DATABASE_H

#include <QWidget>
#include <QTableView>
#include <QSqlRelationalTableModel>
#include <QSqlError>

// Class for database menu
class Database : public QWidget
{
    Q_OBJECT
public:
    explicit Database(QWidget *parent = nullptr);
    ~Database();

private:
    QTableView* table;
    QSqlRelationalTableModel* model;
    QSqlError initDB();

signals:

public slots:
};

#endif // DATABASE_H
