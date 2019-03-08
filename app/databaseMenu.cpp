#include "databaseMenu.h"
#include "dataEntryFactory.h"
#include "enums.h"
#include <sstream>
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
    // Initialize the database
    QSqlError error = initDB();
    if (error.type() != QSqlError::NoError) {
        qDebug() << "Database error: " << error;
    }

    // Enable foreign keys
    QSqlQuery enableQuery("PRAGMA foreign_keys = ON;");
    enableQuery.exec();

    // Get the database fields from a file
    QFile fieldsFile(":/database_fields.rtf");
    fieldsFile.open(QFile::ReadOnly | QFile::Text);
    QTextStream inFields(&fieldsFile);
    QStringList fieldsLine;
    while (!inFields.atEnd())
        dbFields.append((inFields.readLine().split(",")));
    fieldsFile.close();

    // Get the database labels from a file
    QFile labelsFile(":/database_labels.rtf");
    labelsFile.open(QFile::ReadOnly | QFile::Text);
    QTextStream inLabels(&labelsFile);
    while (!inLabels.atEnd())
        columnTitles.append(inLabels.readLine().split(","));
    labelsFile.close();

    // Table view and SQL models
    QFont headerFont("Futura", 14);
    table = new QTableView(this);
    table->setMinimumWidth(400);
    queryModel = new QSqlQueryModel(this);
    table->setModel(queryModel);
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setFont(headerFont);
    table->verticalHeader()->setFont(headerFont);

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
    optionsLayout->addWidget(btnBox, 4);

    // Insert spacing and create push button for adding new record into database
    optionsLayout->addStretch(3);
    addBtn = new QPushButton("Add record");
    addBtn->setMinimumSize(200, 100);
    addBtn->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred, QSizePolicy::PushButton));
    addBtn->setFont(QFont("Gotham", 20, QFont::Medium));
    optionsLayout->addWidget(addBtn, 2);

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
    std::stringstream createTable;

    // Prints table
    createTable << "CREATE TABLE prints ("
                   "id INTEGER PRIMARY KEY,"
                   "date CHAR(10) NOT NULL,"
                   "description VARCHAR(255) NOT NULL,"
                   "experiment SMALLINT CHECK(experiment >= 0),"
                   "dry_time REAL CHECK(dry_time >= 0),"
                   "setup_time REAL CHECK(setup_time >= 0),"
                   "cycle_time REAL CHECK(cycle_time >= 0),"
                   "shutdown_time REAL CHECK(shutdown_time >= 0),"
                   "transition_time REAL CHECK(transition_time >= 0),"
                   "nozzle_temp REAL NOT NULL CHECK(nozzle_temp >= 0),"
                   "spindle_speed REAL NOT NULL CHECK(spindle_speed > 0),"
                   "feed_rate REAL NOT NULL CHECK(feed_rate > 0),"
                   "bed_temp REAL NOT NULL CHECK(bed_temp >= 0 AND bed_temp <= 232),"
                   "dryer_temp REAL CHECK(dryer_temp >= 0),"
                   "dryer_method VARCHAR(50),"
                   "surface_finish VARCHAR(50),"
                   "layer_time REAL,"
                   "rapids INT,"
                   "thermal_video VARCHAR(255),"
                   "visual_video VARCHAR(255));";
    if (!query.exec(createTable.str().c_str()))
        return query.lastError();
    createTable.str(std::string());
    createTable.clear();

    // Tolerances table
    createTable << "CREATE TABLE tolerances ("
                   "id INTEGER PRIMARY KEY,"
                   "print_id INTEGER UNIQUE,"
                   "height REAL CHECK(height > 0),"
                   "width REAL CHECK(width > 0),"
                   "bead_width REAL CHECK(bead_width > 0),"
                   "FOREIGN KEY(print_id) REFERENCES prints(id));";
    if (!query.exec(createTable.str().c_str()))
        return query.lastError();
    createTable.str(std::string());
    createTable.clear();

    // Tensile tests table
    createTable << "CREATE TABLE tensile ("
                   "id INTEGER PRIMARY KEY,"
                   "tolerance_id INTEGER,"
                   "coupon SMALLINT CHECK(coupon >= 1 AND coupon <= 12),"
                   "ultimate REAL,"
                   "percent_elongation REAL,"
                   "yield REAL,"
                   "modulus_elasticity REAL,"
                   "cross_area REAL,"
                   "FOREIGN KEY(tolerance_id) REFERENCES tolerances(id),"
                   "CONSTRAINT ToleranceCoupon UNIQUE (tolerance_id,coupon));";
    if (!query.exec(createTable.str().c_str()))
        return query.lastError();
    createTable.str(std::string());
    createTable.clear();

    // Defects table
    createTable << "CREATE TABLE defects ("
                   "id INTEGER PRIMARY KEY,"
                   "print_id INTEGER UNIQUE,"
                   "description VARCHAR(1023) NOT NULL,"
                   "FOREIGN KEY(print_id) REFERENCES prints(id));";
    if (!query.exec(createTable.str().c_str()))
        return query.lastError();
    createTable.str(std::string());
    createTable.clear();

    // Test insertions into tables
    if (!query.exec("INSERT INTO prints VALUES (NULL,\"2019-02-23\",\"Test 1\",1,1.0,1.25,"
                    "1.50,1.75,2.0,100,200,100,200,300,\"Hopper/dryer\",\"Yes\",30.0,40,"
                    "\"thermal1.avi\",\"visual1.avi\");"))
        return query.lastError();
    if (!query.exec("INSERT INTO prints VALUES (NULL,\"2019-02-27\",\"Test 2\",2,10.0,10.25,"
                    "10.50,10.75,20.0,100,200,100,200,300,\"Hopper/dryer\",\"Yes\",30.0,40,"
                    "\"thermal2.avi\",\"visual2.avi\");"))
        return query.lastError();
    if (!query.exec("INSERT INTO tolerances VALUES (NULL,1,20.0,20.0,1.0);"))
        return query.lastError();
    if (!query.exec("INSERT INTO tolerances VALUES (NULL,2,10.0,10.0,1.0);"))
        return query.lastError();
    for (double i = 1.0; i <= 12.0; i++) {
        QString q = "INSERT INTO tensile VALUES (NULL,1,";
        q += QString::number(int(i)) + ",";
        q += QString::number(i*100) + ",";
        q += QString::number(i*100) + ",";
        q += QString::number(i*100) + ",";
        q += QString::number(i*100) + ",";
        q += QString::number(i) + ");";
        if (!query.exec(q))
            return query.lastError();
        q = "";
    }
    for (double i = 1.0; i <= 12.0; i++) {
        QString q = "INSERT INTO tensile VALUES (NULL,2,";
        q += QString::number(int(i)) + ",";
        q += QString::number(i*1000) + ",";
        q += QString::number(i*1000) + ",";
        q += QString::number(i*1000) + ",";
        q += QString::number(i*1000) + ",";
        q += QString::number(i) + ");";
        if (!query.exec(q))
            return query.lastError();
        q = "";
    }
    if (!query.exec("INSERT INTO defects VALUES (NULL,1,\"Test\");"))
        return query.lastError();
    return query.lastError();
}

DatabaseMenu::~DatabaseMenu()
{

}

void DatabaseMenu::changeTable(int id)
{
    // Titles for this particular set of data
    QStringList titles = columnTitles[id];

    // Select from all tables
    if (id == allID) {
        queryModel->setQuery("SELECT P.*, T.height, T.width, T.bead_width,"
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
        QString query;
        if (id  == printID)
            query = "SELECT * FROM prints;";
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
            for (int i = titles.length() - NUM_TENSILE_TESTS; i < titles.length(); i++)
                titles[i].append(" (Coupons 1-" + QString::number(NUM_COUPONS) + ")");
        }
        else if (id == defectID)
            query = "SELECT * FROM defects;";
        queryModel->setQuery(query);
    }

    // Set the column titles
    for (int i = 0; i < titles.length(); i++)
        queryModel->setHeaderData(i, Qt::Horizontal, titles[i]);
    table->resizeColumnsToContents();
    table->resizeRowsToContents();
}

void DatabaseMenu::openDataDialog()
{
    int id = radioGroup->checkedId();
    if (id == allID) {
        // User must select a specific data set
        QMessageBox selectDialog(QMessageBox::Information, QCoreApplication::applicationName(),
                             "Please select a specific set of data to add\n"
                             "a new record.", QMessageBox::Ok, this);
        selectDialog.exec();
        return;
    }
    recordDialog = DataEntryFactory::createDataEntry(id, columnTitles, this);
    connect(recordDialog, SIGNAL(accepted()), this, SLOT(addRecord()));
    recordDialog->open();
}

void DatabaseMenu::addRecord()
{
    // Get the data from the dialog and the particular table fields
    QStringList data = recordDialog->getData();
    int id = radioGroup->checkedId();
    QStringList fields = dbFields[id];

    // Formulate and execute queries
    QSqlQuery q;
    QString insertQuery = "INSERT INTO ";
    if (id == printID || id == defectID) {
        // Simply insert all data from the dialog into the correct table
        QString table = id == printID ? "prints" : "defects";
        insertQuery.append(" " + table + " (" + fields.join(", ") + ") VALUES (");
        int i;
        for (i = 0; i < fields.length() - 1; i++)
            insertQuery.append(":" + fields[i] + ", ");
        insertQuery.append(":" + fields[i] + ");");
        q.prepare(insertQuery);
        for (int j = 0; j < data.length(); j++)
            q.bindValue(j, data[j]);
        q.exec();
    }
    else if (id == testID) {
        // Get the tensile and tolerance fields and put them in lists
        const int tensileIdx = fields.indexOf("tolerance_id");
        QStringList toleranceFields = fields.mid(0, tensileIdx);
        QStringList tensileFields = fields.mid(tensileIdx);

        // Form and execute query for 'tolerances' table
        insertQuery.append(" tolerances (" + toleranceFields.join(", ") + ") VALUES (");
        int i;
        for (i = 0; i < toleranceFields.length() - 1; i++)
            insertQuery.append(":" + toleranceFields[i] + ", ");
        insertQuery.append(":" + toleranceFields[i] + ");");
        q.prepare(insertQuery);
        int dataIdx;
        for (dataIdx = 0; dataIdx < toleranceFields.length(); dataIdx++)
            q.bindValue(dataIdx, data[dataIdx]);
        q.exec();

        // Get the tolerance ID from the previous insertion
        q.exec("SELECT last_insert_rowid();");
        q.first();
        const int toleranceID = q.value(0).toInt();

        // Form query for 'tensile' table
        insertQuery = "INSERT INTO tensile (" + tensileFields.join(", ") + ")" + "VALUES (";
        for (i = 0; i < tensileFields.length() - 1; i++)
            insertQuery.append(":" + tensileFields[i] + ", ");
        insertQuery.append(":" + tensileFields[i] + ");");
        q.prepare(insertQuery);

        // Gather tensile data and execute a query for each coupon
        QVector<QStringList> tensiles;
        for (i = dataIdx; i < data.length(); i++)
            tensiles.append(data[i].split(", "));
        const int tensileOffset = tensileFields.length() - NUM_TENSILE_TESTS;
        for (i = 0; i < NUM_COUPONS; i++) {
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
