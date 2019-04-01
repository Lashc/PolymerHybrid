#include "dataEntryFactory.h"
#include "databaseColumn.h"
#include "printEntry.h"
#include "testEntry.h"
#include "defectEntry.h"
#include "types.h"
#include <QFile>
#include <QTextStream>

DataEntry* DataEntryFactory::createDataEntry(int type, const QVector<DatabaseColumn*>& DBColumns, QWidget* parent)
{
    // Return the appropiate type of data entry dialog
    switch (type) {
    case printID:
        return new PrintEntry(DBColumns, parent);
    case testID:
        return new TestEntry(DBColumns, parent);
    case defectID:
        return new DefectEntry(DBColumns, parent);
    default:
        return nullptr;
    }
}
