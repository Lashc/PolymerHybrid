#include "foreignKeyValidator.h"
#include <QSqlQuery>
#include <QSqlError>

ForeignKeyValidator::ForeignKeyValidator(const QString& tableName, const QString& foreignTableName, QObject* parent)
    : QValidator (parent), table(tableName), foreignTable(foreignTableName)
{

}

QValidator::State ForeignKeyValidator::validate(QString& input, int& pos) const
{
    int foreignKey = input.toInt();
    if (input.isNull() || input.isEmpty())
        return Intermediate;
    if (foreignKey < 1 || input[0] == '+' || !foreignKey)
        return Invalid;
    QSqlQuery validForeignKey("SELECT MAX(id) FROM " + foreignTable + ";");
    validForeignKey.exec();
    int maxID = 0;
    if (validForeignKey.next())
        maxID = validForeignKey.value(0).toInt();
    else
        return Invalid;
    if (maxID < 1 || foreignKey > maxID)
        return Invalid;
    QString foreignKeyField;
    if (table == "tolerances")
        foreignKeyField = "print_id";
    else if (table == "defects")
        foreignKeyField = "print_id";
    QSqlQuery foreignKeyTaken("SELECT " + foreignKeyField + " FROM " + table + " WHERE " + foreignKeyField + "=" + input + ";");
    QSqlQuery foreignKeyExists("SELECT id FROM " + foreignTable + " WHERE id=" + input + ";");
    foreignKeyTaken.exec();
    foreignKeyExists.exec();
    bool isTaken = foreignKeyTaken.next();
    bool isExistent = foreignKeyExists.next();
    if (!isTaken && isExistent)
        return Acceptable;
    return Intermediate;
}
