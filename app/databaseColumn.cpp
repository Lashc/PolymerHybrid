#include "databaseColumn.h"
#include "dateValidator.h"
#include <limits>

DatabaseColumn::DatabaseColumn(QString fieldFileLine, QObject *parent) : QObject(parent)
{
    QStringList fileColumns = fieldFileLine.split(QRegExp(",\\s*"));
    field = fileColumns[0];
    label = fileColumns[1];
    required = fileColumns[2] == "true" ? true : false;
    QString validatorType = fileColumns[3];
    const int numArgs = fileColumns.length() - 4;
    if (validatorType == "int") {
        // Make sure input is an int and verify its range
        switch(numArgs) {
            case 2:
                validator = new QIntValidator(fileColumns[4].toInt(), fileColumns[5].toInt(), this);
                break;
            case 1:
                validator = new QIntValidator(fileColumns[4].toInt(), 0x7FFFFFFF, this);
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
            validator = new QDoubleValidator(fileColumns[4].toDouble(), fileColumns[5].toDouble(), 3, this);
            break;
        case 1:
            validator = new QDoubleValidator(fileColumns[4].toDouble(), qInf(), 3, this);
            break;
        case 0:
        default:
            validator = new QDoubleValidator(this);
        };
        reinterpret_cast<QDoubleValidator*>(validator)->setNotation(QDoubleValidator::StandardNotation);
    }
    else if (validatorType == "text") {
        // Limit the length of input
        int maxLength = fileColumns[4].toInt();
        validator = new QRegularExpressionValidator(QRegularExpression("^.{0," + QString::number(maxLength) + "}$",
                                                    QRegularExpression::DotMatchesEverythingOption), this);
    }
    else if (validatorType == "date")
        // Verify date format and logicalness
        validator = new DateValidator(this);
    // To be implemented
    else if (validatorType == "foreign_key") validator = nullptr;
    else if (validatorType == "file") validator = nullptr;
    else validator = nullptr;
}
