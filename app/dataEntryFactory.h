#ifndef DATAENTRYFACTORY_H
#define DATAENTRYFACTORY_H

#include <QWidget>

class DataEntry;
struct DatabaseColumn;

// Factory for creating dialogs for adding records to the database
class DataEntryFactory
{
public:
    static DataEntry* createDataEntry(int, const QVector<QVector<DatabaseColumn>>&, QWidget* parent = nullptr);
};

#endif // DATAENTRYFACTORY_H