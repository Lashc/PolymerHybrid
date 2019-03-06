#ifndef DATABASEMENU_H
#define DATABASEMENU_H

#include "dataEntry.h"
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
    QSqlQueryModel* queryModel;
    QTableView* table;
    QButtonGroup* radioGroup;
    QRadioButton* printBtn;
    QRadioButton* testBtn;
    QRadioButton* defectBtn;
    QRadioButton* allBtn;
    QPushButton* addBtn;
    DataEntry* recordDialog;

    QSqlError initDB();
    QSqlError createTables();

signals:

public slots:
    void changeTable(int);
    void openDataDialog();
    void addRecord();
};

#endif // DATABASEMENU_H
