#include "databaseColumn.h"
#include "dateValidator.h"
#include "foreignKeyValidator.h"

DatabaseColumn::DatabaseColumn(const QString& tableName, const QString& fieldFileLine, QObject *parent)
    : QObject(parent), table(tableName)
{
    QStringList fileColumns = fieldFileLine.split(QRegExp(",\\s*"));
    field = fileColumns[0];
    label = fileColumns[1];
    required = fileColumns[2] == "true" ? true : false;
    validatorType = fileColumns[3];
    const int numArgs = fileColumns.length() - 4;
    for (int i = 0; i < numArgs; i++)
        validatorArgs.append(fileColumns[4 + i]);
    if (validatorType == "int") {
        // Make sure input is an int and verify its range
        switch(numArgs) {
            case 2:
                validator = new QIntValidator(validatorArgs[0].toInt(), validatorArgs[1].toInt(), this);
                break;
            case 1:
                validator = new QIntValidator(validatorArgs[0].toInt(), 0x7FFFFFFF, this);
                break;
            case 0:
            default:
                validator = new QIntValidator(this);
        };
    }
    else if (validatorType == "double") {
        // Make sure input is floating point and verify its range
        switch (numArgs) {
        case 2:
            validator = new QDoubleValidator(validatorArgs[0].toDouble(), validatorArgs[1].toDouble(), 3, this);
            break;
        case 1:
            validator = new QDoubleValidator(validatorArgs[0].toDouble(), qInf(), 3, this);
            break;
        case 0:
        default:
            validator = new QDoubleValidator(this);
        };
        reinterpret_cast<QDoubleValidator*>(validator)->setNotation(QDoubleValidator::StandardNotation);
    }
    else if (validatorType == "time") {
        // Verify time is in right format
        validator = new QRegularExpressionValidator(QRegularExpression("^\\d\\d:[0-5][0-9]:[0-5][0-9]$"), this);
    }
    else if (validatorType == "text") {
        // Limit the length of input according to validatorArgs, but
        // don't restrict the possible input
        validator = nullptr;
    }
    else if (validatorType == "date") {
        // Verify date format and logicalness
        validator = new DateValidator(this);
    }
    else if (validatorType == "foreign_key") {
        // Verify that the foreign key exists as the primary key in
        // another table and that it doesn't already exist in this table
        validator = new ForeignKeyValidator(table, fileColumns[4], this);
    }
    else if (validatorType == "file") {
        // Verify filename has acceptable format (with some restrictions)
        validator = new QRegularExpressionValidator(QRegularExpression("^(\\w)+[.]\\w+$"), this);
    }
    else if (validatorType == "none") {
        // Still needs a valid value but may not require automatic validation
        validator = nullptr;
    }
    else {
        // No validator
        validator = nullptr;
    }
}
