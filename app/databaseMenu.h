#ifndef DATABASEMENU_H
#define DATABASEMENU_H

#include <QWidget>
#include <QTableView>
#include <QtSql>

// Class for database menu
class DatabaseMenu : public QWidget
{
    Q_OBJECT
public:
    explicit DatabaseMenu(QWidget *parent = nullptr);
    ~DatabaseMenu();

private:
    QSqlRelationalTableModel* model;
    QTableView* table;

    QSqlError initDB();
    QSqlError createTables();

signals:

public slots:
};

#endif // DATABASEMENU_H
