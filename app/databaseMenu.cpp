#include "databaseMenu.h"
#include <sstream>
#include <QFile>
#include <QDebug>
#include <QDir>

DatabaseMenu::DatabaseMenu(QWidget *parent) : QWidget(parent)
{
    // Initialize the database
    QSqlError error = initDB();

    if (error.type() != QSqlError::NoError) {
        qDebug() << "Database error: " << error;
    }

    // Create the table view and SQL table model
    table = new QTableView(this);
    model = new QSqlRelationalTableModel(this);

    // Testing database
    model->setTable("prints");
    model->select();
    table->setModel(model);
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
    createTable << "CREATE TABLE material_tests (id INTEGER PRIMARY KEY,"
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
    createTable << "CREATE TABLE defects (id INTEGER PRIMARY KEY,"
                   "print_id INTEGER,"
                   "description VARCHAR(1023) NOT NULL,"
                   "FOREIGN KEY(print_id) REFERENCES prints(id));";
    if (!query.exec(createTable.str().c_str()))
        return query.lastError();
    createTable.str(std::string());
    createTable.clear();

    // Test insertion into a table
    if (!query.exec("INSERT INTO prints VALUES (NULL,\"2019-02-23\",\"Test\",1,\"00:00:01\","
                    "200.5,\"01:00\",100,200,100,\"04:00:00\",\"thermal.avi\",\"visual.avi\");"))
        return query.lastError();
    return query.lastError();
}

DatabaseMenu::~DatabaseMenu()
{

}
