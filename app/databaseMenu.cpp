#include "databaseMenu.h"
#include "databaseColumn.h"
#include "dataEntryFactory.h"
#include "dataEntry.h"
#include "entryView.h"
#include "query.h"
#include "types.h"
#include <QtSql>
#include <QTableView>
#include <QButtonGroup>
#include <QRadioButton>
#include <QPushButton>
#include <QFile>
#include <QDir>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>

DatabaseMenu::DatabaseMenu(QWidget *parent)
    : QWidget(parent)
{
    // Initialize the database, enable foreign keys, and populate the database columns
    initDB();
    QSqlQuery enableQuery("PRAGMA foreign_keys = ON;");
    enableQuery.exec();
    setDBColumns();

    // Table view and SQL models
    QFont headerFont("Futura", 14);
    table = new QTableView(this);
    table->setMinimumWidth(400);
    queryModel = new QSqlQueryModel(this);
    table->setModel(queryModel);
    table->setAlternatingRowColors(true);
    QHeaderView* const horizontalHeader = table->horizontalHeader();
    horizontalHeader->setFont(headerFont);
    horizontalHeader->setMaximumSectionSize(450);
    QHeaderView* const verticalHeader = table->verticalHeader();
    verticalHeader->setFont(headerFont);
    verticalHeader->setMinimumWidth(50);
    verticalHeader->setMinimumSectionSize(50);
    verticalHeader->setDefaultAlignment(Qt::AlignCenter);

    // Create radio buttons and add them to a layout
    QFont radioFont("Futura", 20);
    printBtn = new QRadioButton("Print parameters");
    printBtn->setFont(radioFont);
    testBtn = new QRadioButton("Test results");
    testBtn->setFont(radioFont);
    defectBtn = new QRadioButton("Print defects");
    defectBtn->setFont(radioFont);
    allBtn = new QRadioButton("All data");
    allBtn->setFont(radioFont);
    QVBoxLayout* radioLayout = new QVBoxLayout;
    radioLayout->addWidget(printBtn);
    radioLayout->addWidget(testBtn);
    radioLayout->addWidget(defectBtn);
    radioLayout->addWidget(allBtn);

    // Group the radio buttons and put them in a box
    radioGroup = new QButtonGroup(this);
    radioGroup->addButton(printBtn, printID);
    radioGroup->addButton(testBtn, testID);
    radioGroup->addButton(defectBtn, defectID);
    radioGroup->addButton(allBtn, allID);

    QGroupBox* btnBox = new QGroupBox("Select data:");
    btnBox->setFont(QFont("Futura", 28, QFont::Medium));
    btnBox->setMinimumSize(250, 200);
    btnBox->setLayout(radioLayout);

    // Layout for selecting data and performing database operations
    QVBoxLayout* optionsLayout = new QVBoxLayout;
    optionsLayout->addWidget(btnBox, 4);
    optionsLayout->setSpacing(30);

    // Push buttons for adding, modifying, and deleting database entries
    // and for returning to the main menu
    QGridLayout* pushBtnLayout = new QGridLayout;
    pushBtnLayout->setVerticalSpacing(50);
    QSize pushBtnMinSize(180, 120);
    QSizePolicy pushBtnSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum, QSizePolicy::PushButton);
    QFont pushBtnFont("Gotham", 20, QFont::Medium);

    addBtn = new QPushButton("Add entry");
    addBtn->setMinimumSize(pushBtnMinSize);
    addBtn->setSizePolicy(pushBtnSizePolicy);
    addBtn->setFont(pushBtnFont);
    pushBtnLayout->addWidget(addBtn, 0, 0);
    modifyBtn = new QPushButton("Modify entry");
    modifyBtn->setMinimumSize(pushBtnMinSize);
    modifyBtn->setSizePolicy(pushBtnSizePolicy);
    modifyBtn->setFont(pushBtnFont);
    pushBtnLayout->addWidget(modifyBtn, 0, 1);
    deleteBtn = new QPushButton("Delete entry");
    deleteBtn->setMinimumSize(pushBtnMinSize);
    deleteBtn->setSizePolicy(pushBtnSizePolicy);
    deleteBtn->setFont(pushBtnFont);
    pushBtnLayout->addWidget(deleteBtn, 1, 0);
    backBtn = new QPushButton("Back");
    backBtn->setMinimumSize(pushBtnMinSize);
    backBtn->setSizePolicy(pushBtnSizePolicy);
    backBtn->setFont(pushBtnFont);
    pushBtnLayout->addWidget(backBtn, 1, 1);
    optionsLayout->addLayout(pushBtnLayout, 4);

    // Configure main layout
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(table, 11);
    layout->addLayout(optionsLayout, 5);
    layout->setSpacing(25);
    setLayout(layout);

    // 'prints' table is default table selected
    printBtn->setChecked(true);
    setTable(printID);

    // Connect signals and slots
    connect(radioGroup, SIGNAL(buttonPressed(int)), this, SLOT(setTable(int)));
    connect(addBtn, SIGNAL(released()), this, SLOT(addEntry()));
    connect(modifyBtn, SIGNAL(released()), this, SLOT(modifyEntry()));
    connect(deleteBtn, SIGNAL(released()), this, SLOT(deleteEntry()));
    connect(backBtn, SIGNAL(released()), this->parentWidget(), SLOT(returnToMainMenu()));
    connect(verticalHeader, SIGNAL(sectionClicked(int)), this, SLOT(onRowIDClicked(int)));
}

void DatabaseMenu::initDB()
{
    // Initialize the database object
    QString dbName(QDir::homePath() + "/Desktop/" + "polymer_hybrid.db"); // Temporary path for testing
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);

    // Check if the database exists and attempt to open it
    bool dbExists = QFile::exists(dbName);
    db.open();
    if (!dbExists)
        polyDB::createTables();
}

DatabaseMenu::~DatabaseMenu()
{

}

void DatabaseMenu::setTable(int id)
{
    // Labels for the set of data
    QVector<QString> labels;

    // Select from all tables
    if (id == allID) {
        for (int i = printID; i < allID; i++)
            labels.append(getLabels(i));
        labels.removeAll("Print ID");
        labels.prepend("Print ID");

        // Set the query
        queryModel->setQuery("SELECT P.*, T.height, T.width, T.bead_width, T.cross_image,"
                             "(SELECT GROUP_CONCAT(ultimate, \", \") FROM "
                                 "(SELECT TE.ultimate FROM tensiles TE WHERE TE.tolerance_id = T.id ORDER BY TE.coupon)),"
                             "(SELECT GROUP_CONCAT(percent_elongation, \", \") FROM "
                                 "(SELECT TE.percent_elongation FROM tensiles TE WHERE TE.tolerance_id = T.id ORDER BY TE.coupon)),"
                             "(SELECT GROUP_CONCAT(yield, \", \") FROM "
                                 "(SELECT TE.yield FROM tensiles TE WHERE TE.tolerance_id = T.id ORDER BY TE.coupon)),"
                             "(SELECT GROUP_CONCAT(modulus_elasticity, \", \") FROM "
                                 "(SELECT TE.modulus_elasticity FROM tensiles TE WHERE tolerance_id = T.id ORDER BY TE.coupon)),"
                             "(SELECT GROUP_CONCAT(cross_area, \", \") FROM "
                                 "(SELECT TE.cross_area FROM tensiles TE WHERE tolerance_id = T.id ORDER BY TE.coupon)),"
                             "D.description "
                             "FROM prints P "
                             "LEFT JOIN tolerances T ON P.id=T.print_id "
                             "LEFT JOIN defects D on P.id=D.print_id;");
    }

    // Select from one table (or two for test results)
    else {
        // Get the column labels and add an ID one
        labels.reserve(DBColumns[id].size());
        labels = getLabels(id);
        labels.prepend("ID");

        // Set the query based on the data ID
        QString query;
        if (id  == printID) {
            query = "SELECT * FROM prints;";
        }
        else if (id == testID) {
            query = "SELECT T.*,"
                    "(SELECT GROUP_CONCAT(ultimate, \", \") FROM "
                        "(SELECT TE.ultimate FROM tensiles TE WHERE TE.tolerance_id = T.id ORDER BY TE.coupon)),"
                    "(SELECT GROUP_CONCAT(percent_elongation, \", \") FROM "
                        "(SELECT TE.percent_elongation FROM tensiles TE WHERE TE.tolerance_id = T.id ORDER BY TE.coupon)),"
                    "(SELECT GROUP_CONCAT(yield, \", \") FROM "
                        "(SELECT TE.yield FROM tensiles TE WHERE TE.tolerance_id = T.id ORDER BY TE.coupon)),"
                    "(SELECT GROUP_CONCAT(modulus_elasticity, \", \") FROM "
                        "(SELECT TE.modulus_elasticity FROM tensiles TE WHERE tolerance_id = T.id ORDER BY TE.coupon)),"
                    "(SELECT GROUP_CONCAT(cross_area, \", \") FROM "
                        "(SELECT TE.cross_area FROM tensiles TE WHERE tolerance_id = T.id ORDER BY TE.coupon)) "
                    "FROM tolerances T;";
            const int numLabels = labels.length();
            for (int i = numLabels - NUM_TENSILE_TESTS; i < numLabels; i++)
                labels[i].append(" (per coupon)");
        }
        else if (id == defectID)
            query = "SELECT * FROM defects;";
        queryModel->setQuery(query);
    }

    // Set the column titles
    for (int i = 0, numLabels = labels.length(); i < numLabels; i++)
        queryModel->setHeaderData(i, Qt::Horizontal, labels[i]);
    table->resizeColumnsToContents();
    table->resizeRowsToContents();
}

int DatabaseMenu::openEntryDialog(bool isInsertion, QStringList data)
{
    int id = radioGroup->checkedId();
    if (id == allID)
        return QDialog::Rejected;
    entryDialog = DataEntryFactory::createDataEntry(id, DBColumns[id], data, this);
    entryDialog->setWindowTitle(isInsertion ? "Add entry" : "Modify entry");
    return entryDialog->exec();
}

void DatabaseMenu::setDBColumns()
{
    QVector<QString> fieldsFiles( {":/print_fields.rtf", ":/test_fields.rtf", ":/defect_fields.rtf"} );
    QVector<QString> tableNames( {"prints", "tolerances", "tensiles", "defects"} );
    bool reachedTensiles = false;
    for (int i = printID; i <= defectID; i++) {
        QFile fieldsFile(fieldsFiles[i]);
        fieldsFile.open(QFile::ReadOnly | QFile::Text);
        QTextStream inFields(&fieldsFile);
        QStringList fieldsLine;
        QVector<DatabaseColumn*> columns;
        inFields.readLine();
        while (!inFields.atEnd()) {
            QString fieldsLine = inFields.readLine();
            if (i == testID && fieldsLine.contains("tolerance_id"))
                reachedTensiles = true;
            columns.append(new DatabaseColumn(tableNames[i + reachedTensiles], fieldsLine, this));
        }
        DBColumns.append(columns);
        fieldsFile.close();
    }
}

void DatabaseMenu::addEntry()
{
    // Get the data ID
    const int dataID = radioGroup->checkedId();
    if (dataID == allID) {
        // User must select a specific data set
        QMessageBox selectDataDialog(QMessageBox::Information, "Select a data category",
                             "Please select a specific set of data to add "
                             "an entry.", QMessageBox::Ok, this);
        selectDataDialog.exec();
        return;
    }

    // Wait for the user to enter input
    const int code = openEntryDialog(true);
    if (code == QDialog::Rejected) {
        delete entryDialog;
        return;
    }

    // Get the data from the dialog and the particular table fields and create and execute
    // insertion queries against the database
    const QStringList& data = entryDialog->getData();
    const QVector<DatabaseColumn*>& columns = DBColumns[dataID];
    QStringList fields;
    fields.reserve(columns.size());
    foreach(const DatabaseColumn* column, columns)
        fields.append(column->field);
    QSqlError insertError = polyDB::insertEntry(dataID, fields, data);
    if (insertError.type() != QSqlError::NoError) {
        QMessageBox failedInsertionDialog(QMessageBox::Critical, "Database error",
                             "Failed to insert into the database!\n\n"
                             "Error reported:\n"
                             + insertError.text(), QMessageBox::Ok, this);
        failedInsertionDialog.exec();
    }

    // Delete the dialog and refresh the table
    delete entryDialog;
    setTable(dataID);
}

void DatabaseMenu::modifyEntry()
{
    // Get the data ID
    const int dataID = radioGroup->checkedId();
    if (dataID == allID) {
        // Can't modify while viewing all data
        QMessageBox selectDataDialog(QMessageBox::Information, "Select a data category",
                             "Please select a specific set of data to modify "
                             "an entry.", QMessageBox::Ok, this);
        selectDataDialog.exec();
    }

    // Get the existing rows/IDs to choose from
    QStringList IDList;
    for (int i = 0, numRows = queryModel->rowCount(); i < numRows; i++)
        IDList.append(queryModel->record(i).value(0).toString());
    if (IDList.isEmpty())
        return;
    int rowNum = 1;
    QStringList RowIDList;
    foreach(const QString& ID, IDList)
        RowIDList.append(QString::number(rowNum++) + ", " + ID);

    // Let the user choose a row/ID so they can alter the row with that ID if 'OK' is pressed
    bool ok;
    QString row_ID = QInputDialog::getItem(this, "Modify entry", "Select a row/ID:", RowIDList, 0, false, &ok);
    const int row = row_ID.section(", ", 0, 0).toInt();
    const int ID = row_ID.section(", ", 1, 1).toInt();
    if (!ok)
        return;

    // Populate a data entry dialog with the existing data and wait for the user to enter input
    QStringList data;
    for (int i = 1, numFields = queryModel->record(row - 1).count(); i < numFields; i++)
        data.append(queryModel->record(row - 1).value(i).toString());
    const int code = openEntryDialog(false, data);
    if (code == QDialog::Rejected) {
        delete entryDialog;
        return;
    }

    // Update the database with the new data
    data = entryDialog->getData();
    const QVector<DatabaseColumn*>& columns = DBColumns[dataID];
    QStringList fields;
    fields.reserve(columns.size());
    foreach(const DatabaseColumn* column, columns)
        fields.append(column->field);
    QSqlError updateError = polyDB::updateEntry(dataID, fields, data, ID);
    if (updateError.type() != QSqlError::NoError) {
        QMessageBox failedUpdateDialog(QMessageBox::Critical, "Database error",
                             "Failed to update the database!\n\n"
                             "Error reported:\n"
                             + updateError.text(), QMessageBox::Ok, this);
        failedUpdateDialog.exec();
    }

    // Delete the dialog and refresh the table
    delete entryDialog;
    setTable(dataID);
}

void DatabaseMenu::deleteEntry()
{
    // Get the data ID and table to delete from
    const int dataID = radioGroup->checkedId();
    QString tableName;
    switch(dataID) {
    case printID:
        tableName = "prints";
        break;
    case testID:
        tableName = "tolerances";
        break;
    case defectID:
        tableName = "defects";
        break;
    case allID:
    {
        // Can't delete while viewing all data
        QMessageBox selectDataDialog(QMessageBox::Information, "Select a data category",
                             "Please select a specific set of data to delete "
                             "an entry.  To delete all data associated with a print, "
                             "delete the appropriate print entry.", QMessageBox::Ok, this);
        selectDataDialog.exec();
    }
    default:
        return;
    };

    // Get the existing rows/IDs to choose from
    QStringList IDList;
    for (int i = 0, numRows = queryModel->rowCount(); i < numRows; i++)
        IDList.append(queryModel->record(i).value(0).toString());
    if (IDList.isEmpty())
        return;
    int rowNum = 1;
    QStringList RowIDList;
    foreach(const QString& ID, IDList)
        RowIDList.append(QString::number(rowNum++) + ", " + ID);

    // Let the user choose a row/ID and delete the row with that ID if 'OK' is pressed
    bool ok;
    int ID = QInputDialog::getItem(this, "Delete entry", "Select a row/ID:", RowIDList, 0, false, &ok).section(", ", 1, 1).toInt();
    if (!ok)
        return;

    if (dataID == printID) {
        QMessageBox printWarningDialog(QMessageBox::Warning, "Deletion warning",
                             "Warning: deleting a print entry will delete all "
                             "of its associated test results and defect "
                             "information!  Do you wish to continue?", QMessageBox::No | QMessageBox::Yes,
                             this);
        int button = printWarningDialog.exec();
        if (button == QMessageBox::No)
            return;
    }

    QSqlError deleteError = polyDB::deleteEntry(tableName, ID);
    if (deleteError.type() != QSqlError::NoError) {
        QMessageBox failedDeleteDialog(QMessageBox::Critical, "Database error",
                             "Failed to delete from the database!\n\n"
                             "Error reported:\n"
                             + deleteError.text(), QMessageBox::Ok, this);
        failedDeleteDialog.exec();
    }
    setTable(dataID);
}

void DatabaseMenu::onRowIDClicked(int rowNum)
{
    // Can't show a row for all data
    const int id = radioGroup->checkedId();
    if (id == allID) {
        QMessageBox selectDataDialog(QMessageBox::Information, "Select a data category",
                             "Please select a specific set of data to view "
                             "a row of data.", QMessageBox::Ok, this);
        selectDataDialog.exec();
        return;
    }

    // Create a modeless dialog with the data for the row
    EntryView* rowDialog = new EntryView(id, rowNum, getLabels(id), queryModel->record(rowNum), this);
    rowDialog->setAttribute(Qt::WA_DeleteOnClose);
    rowDialog->setWindowTitle("View data");
    rowDialog->show();
    rowDialog->raise();
    rowDialog->activateWindow();
}

QVector<QString> DatabaseMenu::getLabels(int dataID)
{
    QVector<QString> fieldNames;
    foreach(const DatabaseColumn* column, DBColumns[dataID])
        fieldNames.append(column->label);
    fieldNames.removeAll("");
    return fieldNames;
}
