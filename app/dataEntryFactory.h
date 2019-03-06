#ifndef DATAENTRYFACTORY_H
#define DATAENTRYFACTORY_H

#include "printEntry.h"
#include "testEntry.h"
#include "defectEntry.h"

// Factory for creating dialogs for adding records to the database
class DataEntryFactory
{
public:
    static DataEntry* createDataEntry(int, QWidget* parent = nullptr);
};

#endif // DATAENTRYFACTORY_H
