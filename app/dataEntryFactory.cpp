#include "dataEntryFactory.h"
#include "databaseColumn.h"
#include "printEntry.h"
#include "testEntry.h"
#include "defectEntry.h"
#include "types.h"

DataEntry* DataEntryFactory::createDataEntry(int type, const QVector<DatabaseColumn*>& DBColumns, QStringList data, QWidget* parent)
{
    // Return the appropiate type of data entry dialog
    switch (type) {
    case printID:
        if (data.isEmpty())
            return new PrintEntry(DBColumns, parent);
        else
            return new PrintEntry(DBColumns, data, parent);
    case testID:
        if (data.isEmpty())
            return new TestEntry(DBColumns, parent);
        else
            return new TestEntry(DBColumns, data, parent);
    case defectID:
        if (data.isEmpty())
            return new DefectEntry(DBColumns, parent);
        else
            return new DefectEntry(DBColumns, data, parent);
    default:
        return nullptr;
    }
}
