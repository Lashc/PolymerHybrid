#ifndef DATABASEMENU_H
#define DATABASEMENU_H

#include <QWidget>

struct DatabaseColumn;
class QSqlQueryModel;
class QTableView;
class QLabel;
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
    QPushButton* backBtn;
    QPushButton* addBtn;
    DataEntry* recordDialog;

    QSqlError initDB();
    void setDBColumns();
    QSqlError createTables();

signals:
    void returnToMainMenu();

public slots:
    void changeTable(int);
    void openDataDialog();
    void addEntry();
    void onRowIDClicked(int);
};

#endif // DATABASEMENU_H
