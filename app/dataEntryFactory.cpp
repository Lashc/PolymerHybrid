#include "dataEntryFactory.h"
#include "enums.h"
#include <QFile>
#include <QTextStream>

DataEntry* DataEntryFactory::createDataEntry(int type, const QVector<QStringList>& labels, QWidget* parent)
{
    // Return the appropiate type of data entry dialog
    switch (type) {
    case printID:
        return new PrintEntry(labels[type], parent);
    case testID:
        return new TestEntry(labels[type], parent);
    case defectID:
        return new DefectEntry(labels[type], parent);
    default:
        return nullptr;
    }
}
