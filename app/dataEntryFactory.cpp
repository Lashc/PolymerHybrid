#include "dataEntryFactory.h"
#include "printEntry.h"
#include "testEntry.h"
#include "defectEntry.h"
#include "types.h"
#include <QFile>
#include <QTextStream>

DataEntry* DataEntryFactory::createDataEntry(int type, const QVector<QVector<DatabaseColumn>>& DBColumns, QWidget* parent)
{
    // Get the labels for the category of data and remove any empty ones
    QStringList labels;
    const QVector<DatabaseColumn>& columns = DBColumns[type];
    foreach(const DatabaseColumn& column, columns) {
        labels.append(column.label);
    }
    labels.removeAll("");

    // Return the appropiate type of data entry dialog
    switch (type) {
    case printID:
        return new PrintEntry(labels, parent);
    case testID:
        return new TestEntry(labels, parent);
    case defectID:
        return new DefectEntry(labels, parent);
    default:
        return nullptr;
    }
}
