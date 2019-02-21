#include "database.h"
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QSqlQuery>

Database::Database(QWidget *parent) : QWidget(parent)
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
    model->setTable("test");
    model->select();
    table->setModel(model);
}

Database::~Database()
{

}

QSqlError Database::initDB() {
    // Initialize the database object
    QString dbName(QDir::homePath() + "/Desktop/" + "polymer_hybrid.db"); // Temporary path for testing
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);

    // Check if the database exists and open it
    bool dbExists = QFile::exists(dbName);
    if (!db.open())
        return db.lastError();
    if (!dbExists) {
        // Create tables
        QSqlQuery query;

        // Test queries
        query.exec("CREATE TABLE test (col integer primary key);");
        query.exec("INSERT INTO test VALUES (1)");
    }
    return db.lastError();
}
