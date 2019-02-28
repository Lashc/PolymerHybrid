#ifndef DATABASEMENU_H
#define DATABASEMENU_H

#include <QWidget>
#include <QTableView>
#include <QtSql>
#include <QRadioButton>
#include <QPushButton>
#include <QButtonGroup>

// Class for database menu
class DatabaseMenu : public QWidget
{
    Q_OBJECT
public:
    explicit DatabaseMenu(QWidget *parent = nullptr);
    ~DatabaseMenu();

private:
    QSqlRelationalTableModel* model;
    QSqlQueryModel* joinModel;
    QTableView* table;
    QButtonGroup* radioGroup;
    QRadioButton* printBtn;
    QRadioButton* testBtn;
    QRadioButton* defectBtn;
    QRadioButton* allBtn;
    QPushButton* addBtn;

    QSqlError initDB();
    QSqlError createTables();

signals:

public slots:
    void changeTable(int);
};

#endif // DATABASEMENU_H
