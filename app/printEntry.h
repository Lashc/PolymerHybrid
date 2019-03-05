#ifndef PRINTENTRY_H
#define PRINTENTRY_H

#include "dataEntry.h"

// Dialog class for adding new print entries to the 'prints' table of the database
class PrintEntry : public DataEntry
{
    Q_OBJECT
public:
    explicit PrintEntry(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

private:
    virtual QList<QString> createLabels();
    virtual void setDBFields();
};

#endif // PRINTENTRY_H
