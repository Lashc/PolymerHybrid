#ifndef DATABASEMENU_H
#define DATABASEMENU_H

#include <QWidget>

struct DatabaseColumn;
class QSqlQueryModel;
class QTableView;
class QButtonGroup;
class QRadioButton;
class QPushButton;
class DataEntry;
class QSqlError;

// Class for database menu
class DatabaseMenu : public QWidget
{
    Q_OBJECT
public:
    explicit DatabaseMenu(QWidget *parent = nullptr);
    ~DatabaseMenu();

private:
    QVector<QVector<DatabaseColumn>> DBColumns;
    QSqlQueryModel* queryModel;
    QTableView* table;
    QButtonGroup* radioGroup;
    QRadioButton* printBtn;
    QRadioButton* testBtn;
    QRadioButton* defectBtn;
    QRadioButton* allBtn;
    QPushButton* addBtn;
    DataEntry* recordDialog;
    QDialog* showData;

    QSqlError initDB();
    void setDBColumns();
    QSqlError createTables();

signals:

public slots:
    void changeTable(int);
    void openDataDialog();
    void addRecord();
    void rowIDClicked(int);
};

#endif // DATABASEMENU_H
