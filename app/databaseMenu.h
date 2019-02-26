#ifndef DATABASEMENU_H
#define DATABASEMENU_H

#include <QWidget>
#include <QTableView>
#include <QtSql>
#include <QRadioButton>
#include <QPushButton>

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
    QRadioButton* printBtn;
    QRadioButton* testBtn;
    QRadioButton* defectBtn;
    QRadioButton* allBtn;
    QPushButton* addBtn;

    QSqlError initDB();
    QSqlError createTables();

signals:

public slots:
};

#endif // DATABASEMENU_H
