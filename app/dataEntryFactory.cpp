#include "dataEntryFactory.h"
#include <QFile>
#include <QTextStream>

DataEntry* DataEntryFactory::createDataEntry(int type, QWidget* parent)
{
    // Get the database fields from a file
    QFile fieldsFile(":/database_fields.rtf");
    fieldsFile.open(QFile::ReadOnly | QFile::Text);
    QTextStream inFields(&fieldsFile);
    QList<QString> fieldsLines;
    while (!inFields.atEnd())
        fieldsLines.append(inFields.readLine());
    QList<QString> fields = fieldsLines[type].split(",");
    fieldsFile.close();

    // Get the database labels from a file
    QFile labelsFile(":/database_labels.rtf");
    labelsFile.open(QFile::ReadOnly | QFile::Text);
    QTextStream inLabels(&labelsFile);
    QList<QString> labelsLines;
    while (!inLabels.atEnd())
        labelsLines.append(inLabels.readLine());
    QList<QString> labels = labelsLines[type].split(",");
    labelsFile.close();

    // Return the appropiate type of data entry dialog
    switch (type) {
    case 0:
        return new PrintEntry(fields, labels, parent);
    case 1:
        return new TestEntry(fields, labels, parent);
    case 2:
        return new DefectEntry(fields, labels, parent);
    }
}
