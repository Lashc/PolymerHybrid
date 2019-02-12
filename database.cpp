#include "database.h"

Database::Database(QWidget *parent) : QWidget(parent)
{
    title = new QLabel("The database", this);
    title->setFont(QFont("Futura", 50));
}

Database::~Database()
{

}
