#include "dataEntryFactory.h"
#include "enums.h"
#include <QFile>
#include <QTextStream>

DataEntry* DataEntryFactory::createDataEntry(int type, const QVector<QVector<QString>>& fields,
                                             const QVector<QVector<QString>>& labels, QWidget* parent)
{
    // Return the appropiate type of data entry dialog
    switch (type) {
    case printID:
        return new PrintEntry(fields[type], labels[type], parent);
    case testID:
        return new TestEntry(fields[type], labels[type], parent);
    case defectID:
        return new DefectEntry(fields[type], labels[type], parent);
    default:
        return nullptr;
    }
}
