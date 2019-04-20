#ifndef DATABASEMENU_H
#define DATABASEMENU_H

#include <QWidget>
#include "databaseColumn.h"

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
    QVector<QVector<DatabaseColumn*>> DBColumns;
    QSqlQueryModel* queryModel;
    QTableView* table;
    QButtonGroup* radioGroup;
    QRadioButton* printBtn;
    QRadioButton* testBtn;
    QRadioButton* defectBtn;
    QRadioButton* allBtn;
    QPushButton* addBtn;
    QPushButton* modifyBtn;
    QPushButton* deleteBtn;
    QPushButton* backBtn;
    DataEntry* entryDialog;

    void initDB();
    void setDBColumns();
    QVector<QString> getLabels(int);
    int openEntryDialog(bool, QStringList data = QStringList());

signals:

public slots:
    void setTable(int);
    void addEntry();
    void modifyEntry();
    void deleteEntry();
    void onRowIDClicked(int);
};

#endif // DATABASEMENU_H
