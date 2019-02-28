#include "databaseMenu.h"
#include <sstream>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

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
    model = new QSqlRelationalTableModel(this);
    joinModel = new QSqlQueryModel(this);
    table->setModel(model);

    // Configure foreign key references
    model->setTable("material_tests");
    model->setRelation(1, QSqlRelation("prints", "id", "print_id"));
    model->setTable("defects");
    model->setRelation(1, QSqlRelation("prints", "id", "print_id"));
    model->setJoinMode(QSqlRelationalTableModel::LeftJoin);

    // Layouts
    QHBoxLayout* layout = new QHBoxLayout;
    QVBoxLayout* radioLayout = new QVBoxLayout;
    QVBoxLayout* optionsLayout = new QVBoxLayout;

    // Create radio buttons and add them to a layout
    QFont radioFont("Futura", 15);
    printBtn = new QRadioButton("Print parameters");
    printBtn->setFont(radioFont);
    testBtn = new QRadioButton("Material testing");
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
    radioGroup->addButton(printBtn, 1);
    radioGroup->addButton(testBtn, 2);
    radioGroup->addButton(defectBtn, 3);
    radioGroup->addButton(allBtn, 4);

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
    model->setTable("prints");
    model->select();

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
    QSqlQuery query;
    std::stringstream createTable;

    // Form and execute queries for creating tables
    // Prints table
    createTable << "CREATE TABLE prints"
                   "(id INTEGER PRIMARY KEY,"
                   "date CHAR(10) NOT NULL,"
                   "description VARCHAR(255) NOT NULL,"
                   "experiment SMALLINT CHECK(experiment >= 0),"
                   "total_time CHAR(8),"
                   "nozzle_temp REAL NOT NULL CHECK(nozzle_temp >= 0),"
                   "layer_time CHAR(5),"
                   "feed_rate REAL NOT NULL CHECK(feed_rate > 0),"
                   "spindle_speed REAL NOT NULL CHECK(spindle_speed > 0),"
                   "bed_temp REAL NOT NULL CHECK(bed_temp >= 0 AND bed_temp <= 232),"
                   "dryer_time CHAR(8),"
                   "thermal_video VARCHAR(255),"
                   "visual_video VARCHAR(255));";
    if (!query.exec(createTable.str().c_str()))
        return query.lastError();
    createTable.str(std::string());
    createTable.clear();

    // Material tests table
    createTable << "CREATE TABLE material_tests ("
                   "id INTEGER PRIMARY KEY,"
                   "print_id INTEGER,"
                   "coupon SMALLINT CHECK(coupon >= 1 AND coupon <=12),"
                   "ultimate_tensile REAL,"
                   "yield REAL,"
                   "modulus_elasticity REAL,"
                   "percent_elongation REAL,"
                   "FOREIGN KEY(print_id) REFERENCES prints(id));";
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
    if (!query.exec("INSERT INTO prints VALUES (NULL,\"2019-02-23\",\"Test 1\",1,\"00:00:01\","
                    "200.5,\"01:00\",100,200,100,\"04:00:00\",\"thermal1.avi\",\"visual1.avi\");"))
        return query.lastError();
    if (!query.exec("INSERT INTO prints VALUES (NULL,\"2019-02-27\",\"Test 2\",2,\"00:00:01\","
                     "50,\"01:00\",50,50,50,\"04:00:00\",\"thermal2.avi\",\"visual2.avi\");"))
        return query.lastError();
    if (!query.exec("INSERT INTO material_tests VALUES (NULL,1,2,100,200,300,400);"))
        return query.lastError();
    if (!query.exec("INSERT INTO defects VALUES (NULL,1,\"Test\");"))
        return query.lastError();
    return query.lastError();
}

DatabaseMenu::~DatabaseMenu()
{

}

void DatabaseMenu::changeTable(int id)
{
    // Select from all tables
    if (id > 3) {
        joinModel->setQuery("SELECT P.*, M.*, D.* FROM prints P "
                    "LEFT JOIN material_tests M ON P.id=M.print_id "
                    "LEFT JOIN defects D on M.print_id = D.print_id;");
        table->setModel(joinModel);
    }

    // Select from one table
    else {
        table->setModel(model);
        QString tableName;
        tableName = id == 1 ? "prints" : id == 2 ? "material_tests" : "defects";
        model->setTable(tableName);
        model->select();
    }
}
