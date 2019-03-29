#include "databaseMenu.h"
#include "dataEntryFactory.h"
#include "dataEntry.h"
#include "entryView.h"
#include "types.h"
#include <QtSql>
#include <QTableView>
#include <QButtonGroup>
#include <QRadioButton>
#include <QPushButton>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>

DatabaseMenu::DatabaseMenu(QWidget *parent) : QWidget(parent)
{
    // Initialize the database, enable foreign keys, and populate the database columns
    QSqlError error = initDB();
    if (error.type() != QSqlError::NoError) {
        qDebug() << "Database error: " << error;
    }
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
    horizontalHeader->setMaximumSectionSize(400);
    QHeaderView* const verticalHeader = table->verticalHeader();
    verticalHeader->setFont(headerFont);
    verticalHeader->setMinimumWidth(50);
    verticalHeader->setMinimumSectionSize(50);
    verticalHeader->setDefaultAlignment(Qt::AlignCenter);

    // Create radio buttons and add them to a layout
    QFont radioFont("Futura", 15);
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
    btnBox->setFont(QFont("Futura", 25, QFont::Medium));
    btnBox->setMinimumSize(250, 200);
    btnBox->setLayout(radioLayout);

    // Layout for selecting data and adding entries
    QVBoxLayout* optionsLayout = new QVBoxLayout;
    optionsLayout->addWidget(btnBox, 5);
    optionsLayout->setSpacing(30);

    // Insert spacing and create push button for adding new records into the database
    optionsLayout->addStretch(2);
    addBtn = new QPushButton("Add entry");
    addBtn->setMinimumSize(200, 100);
    addBtn->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred, QSizePolicy::PushButton));
    addBtn->setFont(QFont("Gotham", 20, QFont::Medium));
    optionsLayout->addWidget(addBtn, 2);
    backBtn = new QPushButton("Back");
    backBtn->setMinimumSize(200, 100);
    backBtn->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred, QSizePolicy::PushButton));
    backBtn->setFont(QFont("Gotham", 20, QFont::Medium));
    optionsLayout->addWidget(backBtn, 2);

    // Configure main layout
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(table, 5);
    layout->addLayout(optionsLayout, 2);
    setLayout(layout);

    // 'prints' table is default table selected
    printBtn->setChecked(true);
    changeTable(printID);

    // Connect signals and slots
    connect(radioGroup, SIGNAL(buttonPressed(int)), this, SLOT(changeTable(int)));
    connect(addBtn, SIGNAL(released()), this, SLOT(openDataDialog()));
    connect(backBtn, SIGNAL(released()), this->parentWidget(), SLOT(returnToMainMenu()));
    connect(verticalHeader, SIGNAL(sectionClicked(int)), this, SLOT(onRowIDClicked(int)));
}

QSqlError DatabaseMenu::initDB()
{
    // Initialize the database object
    QString dbName(QDir::homePath() + "/Desktop/" + "polymer_hybrid.db"); // Temporary path for testing
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);

    // Check if the database exists and attempt to open it
    bool dbExists = QFile::exists(dbName);
    if (!db.open())
        return db.lastError();
    if (!dbExists)
        return createTables();
    return db.lastError();
}

QSqlError DatabaseMenu::createTables()
{
    // Form and execute queries for creating tables
    QSqlQuery query;
    QString createTable;

    // Prints table
    createTable = "CREATE TABLE prints ("
                  "id INTEGER PRIMARY KEY,"
                  "description VARCHAR(127) NOT NULL,"
                  "date CHAR(10) NOT NULL,"
                  "spindle_speed REAL NOT NULL CHECK(spindle_speed > 0),"
                  "feed_rate REAL NOT NULL CHECK(feed_rate > 0),"
                  "layer_height REAL NOT NULL CHECK(layer_height > 0),"
                  "rapids SMALLINT NOT NULL CHECK(rapids > 0),"
                  "nozzle_temp REAL NOT NULL CHECK(nozzle_temp >= 0),"
                  "bed_temp REAL NOT NULL CHECK(bed_temp >= 0 AND bed_temp <= 232),"
                  "dryer_temp REAL CHECK(dryer_temp >= 0),"
                  "ambient_temp REAL CHECK(ambient_temp >= 0),"
                  "humidity TINYINT CHECK(humidity >= 0 AND humidity <= 100),"
                  "surface_finish REAL CHECK(surface_finish >= 0),"
                  "cycle_time CHAR(8),"
                  "layer_time CHAR(8),"
                  "experiment SMALLINT CHECK(experiment >= 0),"
                  "dry_time CHAR(8),"
                  "setup_time CHAR(8),"
                  "shutdown_time CHAR(8),"
                  "transition_time CHAR(8),"
                  "video VARCHAR(255),"
                  "notes VARCHAR(1023));";
    if (!query.exec(createTable))
        return query.lastError();

    // Tolerances table
    createTable = "CREATE TABLE tolerances ("
                  "id INTEGER PRIMARY KEY,"
                  "print_id INTEGER UNIQUE,"
                  "height REAL CHECK(height > 0),"
                  "width REAL CHECK(width > 0),"
                  "bead_width REAL CHECK(bead_width > 0),"
                  "cross_image VARCHAR(255),"
                  "FOREIGN KEY(print_id) REFERENCES prints(id));";
    if (!query.exec(createTable))
        return query.lastError();

    // Tensile tests table
    createTable = "CREATE TABLE tensile ("
                  "id INTEGER PRIMARY KEY,"
                  "tolerance_id INTEGER,"
                  "coupon TINYINT CHECK(coupon >= 1 AND coupon <= 12),"
                  "ultimate REAL,"
                  "percent_elongation REAL,"
                  "yield REAL,"
                  "modulus_elasticity REAL,"
                  "cross_area REAL,"
                  "FOREIGN KEY(tolerance_id) REFERENCES tolerances(id),"
                  "CONSTRAINT ToleranceCoupon UNIQUE (tolerance_id,coupon));";
    if (!query.exec(createTable))
        return query.lastError();

    // Defects table
    createTable = "CREATE TABLE defects ("
                  "id INTEGER PRIMARY KEY,"
                  "print_id INTEGER UNIQUE,"
                  "description VARCHAR(1023) NOT NULL,"
                  "FOREIGN KEY(print_id) REFERENCES prints(id));";
    query.exec(createTable);
    return query.lastError();
}

DatabaseMenu::~DatabaseMenu()
{

}

void DatabaseMenu::changeTable(int id)
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
                                 "(SELECT TE.ultimate FROM tensile TE WHERE TE.tolerance_id = T.id ORDER BY TE.coupon)),"
                             "(SELECT GROUP_CONCAT(percent_elongation, \", \") FROM "
                                 "(SELECT TE.percent_elongation FROM tensile TE WHERE TE.tolerance_id = T.id ORDER BY TE.coupon)),"
                             "(SELECT GROUP_CONCAT(yield, \", \") FROM "
                                 "(SELECT TE.yield FROM tensile TE WHERE TE.tolerance_id = T.id ORDER BY TE.coupon)),"
                             "(SELECT GROUP_CONCAT(modulus_elasticity, \", \") FROM "
                                 "(SELECT TE.modulus_elasticity FROM tensile TE WHERE tolerance_id = T.id ORDER BY TE.coupon)),"
                             "(SELECT GROUP_CONCAT(cross_area, \", \") FROM "
                                 "(SELECT TE.cross_area FROM tensile TE WHERE tolerance_id = T.id ORDER BY TE.coupon)),"
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
                        "(SELECT TE.ultimate FROM tensile TE WHERE TE.tolerance_id = T.id ORDER BY TE.coupon)),"
                    "(SELECT GROUP_CONCAT(percent_elongation, \", \") FROM "
                        "(SELECT TE.percent_elongation FROM tensile TE WHERE TE.tolerance_id = T.id ORDER BY TE.coupon)),"
                    "(SELECT GROUP_CONCAT(yield, \", \") FROM "
                        "(SELECT TE.yield FROM tensile TE WHERE TE.tolerance_id = T.id ORDER BY TE.coupon)),"
                    "(SELECT GROUP_CONCAT(modulus_elasticity, \", \") FROM "
                        "(SELECT TE.modulus_elasticity FROM tensile TE WHERE tolerance_id = T.id ORDER BY TE.coupon)),"
                    "(SELECT GROUP_CONCAT(cross_area, \", \") FROM "
                        "(SELECT TE.cross_area FROM tensile TE WHERE tolerance_id = T.id ORDER BY TE.coupon)) "
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

void DatabaseMenu::openDataDialog()
{
    int id = radioGroup->checkedId();
    if (id == allID) {
        // User must select a specific data set
        QMessageBox selectDataDialog(QMessageBox::Information, "Select a data category",
                             "Please select a specific set of data to add\n"
                             "a new entry.", QMessageBox::Ok, this);
        selectDataDialog.exec();
        return;
    }
    recordDialog = DataEntryFactory::createDataEntry(id, DBColumns, this);
    connect(recordDialog, SIGNAL(accepted()), this, SLOT(addEntry()));
    recordDialog->open();
}

void DatabaseMenu::setDBColumns()
{
    QVector<QString> fieldsFiles( {":/print_fields.rtf", ":/test_fields.rtf", ":/defect_fields.rtf"} );
    for (int i = printID; i <= defectID; i++) {
        QFile fieldsFile(fieldsFiles[i]);
        fieldsFile.open(QFile::ReadOnly | QFile::Text);
        QTextStream inFields(&fieldsFile);
        QStringList fieldsLine;
        QVector<DatabaseColumn> columns;
        while (!inFields.atEnd()) {
            fieldsLine = inFields.readLine().split(",");
            columns.append( {fieldsLine[0], fieldsLine[1]} );
        }
        DBColumns.append(columns);
        fieldsFile.close();
    }
}

void DatabaseMenu::addEntry()
{
    // Get the data from the dialog and the particular table fields
    const QStringList& data = recordDialog->getData();
    const int numData = data.length();
    const int id = radioGroup->checkedId();
    const QVector<DatabaseColumn>& columns = DBColumns[id];
    QStringList fields;
    fields.reserve(columns.size());
    foreach(const DatabaseColumn& column, columns)
        fields.append(column.field);
    const int numFields = fields.length();

    // Formulate and execute queries
    QSqlQuery q;
    QString insertQuery = "INSERT INTO ";
    if (id == printID || id == defectID) {
        // Simply insert all data from the dialog into the correct table
        QString table = id == printID ? "prints" : "defects";
        insertQuery.append(table + " (" + fields.join(", ") + ") VALUES (");
        for (int i = 0; i < numFields - 1; i++)
            insertQuery.append(":" + fields[i] + ", ");
        insertQuery.append(":" + fields[numFields - 1] + ");");
        q.prepare(insertQuery);
        for (int i = 0; i < numData; i++)
            q.bindValue(i, data[i]);
        q.exec();
    }
    else if (id == testID) {
        // Get the tensile and tolerance fields and put them in lists
        const int tensileIdx = fields.indexOf("tolerance_id");
        QStringList toleranceFields = fields.mid(0, tensileIdx);
        QStringList tensileFields = fields.mid(tensileIdx);

        // Form and execute query for 'tolerances' table
        insertQuery.append(" tolerances (" + toleranceFields.join(", ") + ") VALUES (");
        const int numTolerances = toleranceFields.length();
        for (int i = 0; i < numTolerances - 1; i++)
            insertQuery.append(":" + toleranceFields[i] + ", ");
        insertQuery.append(":" + toleranceFields[numTolerances - 1] + ");");
        q.prepare(insertQuery);
        for (int i = 0; i < numTolerances; i++)
            q.bindValue(i, data[i]);
        q.exec();

        // Get the tolerance ID from the previous insertion
        q.exec("SELECT last_insert_rowid();");
        q.first();
        const int toleranceID = q.value(0).toInt();

        // Form query for 'tensile' table
        const int numTensiles = tensileFields.length();
        insertQuery = "INSERT INTO tensile (" + tensileFields.join(", ") + ")" + "VALUES (";
        for (int i = 0; i < numTensiles - 1; i++)
            insertQuery.append(":" + tensileFields[i] + ", ");
        insertQuery.append(":" + tensileFields[numTensiles - 1] + ");");
        q.prepare(insertQuery);

        // Gather tensile data and execute a query for each coupon
        QVector<QStringList> tensiles;
        const int tensileOffset = tensileFields.length() - NUM_TENSILE_TESTS;
        for (int i = numTolerances; i < data.length(); i++)
            tensiles.append(data[i].split(", "));
        for (int i = 0; i < NUM_COUPONS; i++) {
            q.bindValue(0, toleranceID);
            q.bindValue(1, i + 1);
            for (int j = 0; j < NUM_TENSILE_TESTS; j++)
                q.bindValue(j + tensileOffset, tensiles[i][j]);
            q.exec();
        }
    }

    // Delete the dialog and refresh the table
    delete recordDialog;
    changeTable(id);
}

void DatabaseMenu::onRowIDClicked(int rowNum)
{
    // Can't show a row for all data
    const int id = radioGroup->checkedId();
    if (id == allID) {
        QMessageBox selectDataDialog(QMessageBox::Information, "Select a data category",
                             "Please select a specific set of data to view\n"
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
    foreach(const DatabaseColumn& column, DBColumns[dataID])
        fieldNames.append(column.label);
    fieldNames.removeAll("");
    return fieldNames;
}
