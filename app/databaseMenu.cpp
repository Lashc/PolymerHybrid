#include "databaseMenu.h"
#include <sstream>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

// Table IDs used for switching between tables
enum TableID {
    printID,
    testID,
    defectID,
    allID
};

DatabaseMenu::DatabaseMenu(QWidget *parent) : QWidget(parent)
{
    // Initialize the database
    QSqlError error = initDB();
    if (error.type() != QSqlError::NoError) {
        qDebug() << "Database error: " << error;
    }

    // Table view and SQL models
    table = new QTableView(this);
    table->setMinimumWidth(400);
    queryModel = new QSqlQueryModel(this);
    table->setModel(queryModel);

    // Layouts
    QHBoxLayout* layout = new QHBoxLayout;
    QVBoxLayout* radioLayout = new QVBoxLayout;
    QVBoxLayout* optionsLayout = new QVBoxLayout;

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
    optionsLayout->addWidget(btnBox, 4);

    // Insert spacing and create push button for adding new record into database
    optionsLayout->addStretch(3);
    addBtn = new QPushButton("Add entry");
    addBtn->setMinimumSize(200, 100);
    addBtn->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred, QSizePolicy::PushButton));
    addBtn->setFont(QFont("Gotham", 20, QFont::Medium));
    optionsLayout->addWidget(addBtn, 2);

    // Configure main layout
    layout->addWidget(table, 5);
    layout->addLayout(optionsLayout, 2);
    setLayout(layout);

    // 'prints' table is default table selected
    printBtn->setChecked(1);
    changeTable(printID);

    // Connect signals and slots
    connect(radioGroup, SIGNAL(buttonPressed(int)), this, SLOT(changeTable(int)));
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
                   "print_id INTEGER,"
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
                   "FOREIGN KEY(tolerance_id) REFERENCES tolerances(id));";
    if (!query.exec(createTable.str().c_str()))
        return query.lastError();
    createTable.str(std::string());
    createTable.clear();

    // Defects table
    createTable << "CREATE TABLE defects ("
                   "id INTEGER PRIMARY KEY,"
                   "print_id INTEGER,"
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
    // List of nicer looking titles
    QList<QString> columnTitles;

    // Select from all tables
    if (id == allID) {
        queryModel->setQuery("SELECT P.*, T.height, T.width, T.bead_width,"
                             "(SELECT GROUP_CONCAT(ultimate, \", \") FROM tensile WHERE tolerance_id = T.id),"
                             "(SELECT GROUP_CONCAT(percent_elongation, \", \") FROM tensile WHERE tolerance_id = T.id),"
                             "(SELECT GROUP_CONCAT(yield, \", \") FROM tensile WHERE tolerance_id = T.id),"
                             "(SELECT GROUP_CONCAT(modulus_elasticity, \", \") FROM tensile WHERE tolerance_id = T.id),"
                             "(SELECT GROUP_CONCAT(cross_area, \", \") FROM tensile WHERE tolerance_id = T.id),"
                             "D.description "
                             "FROM prints P "
                             "LEFT JOIN tolerances T ON P.id=T.print_id "
                             "LEFT JOIN defects D on T.print_id=D.print_id;");
        columnTitles = { "Print ID", "Date", "Description", "Experiment", "Drying time",
                         "Setup time", "Cycle time", "Shut down time", "Transition time",
                         "Nozzle temperature (°C)", "Spindle speeed", "Feed rate",
                         "Bed temperature (°C)", "Dryer temperature (°C)", "Dryer method",
                         "Surface finish", "Layer time","Rapid %", "Thermal video", "Visual video",
                         "Height", "Width", "Bead width", "Ultimate tensile strength (Coupons 1-12)",
                         "Yield strength (Coupons 1-12)", "Modulus of elasticity (Coupons 1-12)",
                         "Percent elongation (Coupons 1-12)", "Cross sectional area (Coupons 1-12)",
                         "Defect description" };
    }

    // Select from one table (or two for test results)
    else {
        QString query;
        if (id  == printID) {
            query = "SELECT * FROM prints;";
            columnTitles = { "ID", "Date", "Description", "Experiment", "Drying time",
                             "Setup time", "Cycle time", "Shut down time", "Transition time",
                             "Nozzle temperature (°C)", "Spindle speeed", "Feed rate",
                             "Bed temperature (°C)", "Dryer temperature (°C)", "Dryer method",
                             "Surface finish", "Layer time","Rapid %", "Thermal video", "Visual video" };
        }

        else if (id == testID) {
            query = "SELECT T.*,"
                    "(SELECT GROUP_CONCAT(ultimate, \", \") FROM tensile WHERE tolerance_id = T.id),"
                    "(SELECT GROUP_CONCAT(percent_elongation, \", \") FROM tensile WHERE tolerance_id = T.id),"
                    "(SELECT GROUP_CONCAT(yield, \", \") FROM tensile WHERE tolerance_id = T.id),"
                    "(SELECT GROUP_CONCAT(modulus_elasticity, \", \") FROM tensile WHERE tolerance_id = T.id),"
                    "(SELECT GROUP_CONCAT(cross_area, \", \") FROM tensile WHERE tolerance_id = T.id) "
                    "FROM tolerances T";
            columnTitles = { "ID", "Print ID", "Height", "Width", "Bead width",
                             "Ultimate tensile strength (Coupons 1-12)", "Yield strength (Coupons 1-12)",
                             "Modulus of elasticity (Coupons 1-12)", "Percent elongation (Coupons 1-12)",
                             "Cross sectional area (Coupons 1-12)" };
        }

        else if (id == defectID) {
            query = "SELECT * FROM defects;";
            columnTitles = { "ID", "Print ID", "Description" };
        }
        queryModel->setQuery(query);
    }

    // Set the column titles
    for (int i = 0; i < columnTitles.length(); i++)
        queryModel->setHeaderData(i, Qt::Horizontal, columnTitles[i]);
    table->resizeColumnsToContents();
    table->resizeRowsToContents();
}
