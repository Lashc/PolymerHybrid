#ifndef DEFECTENTRY_H
#define DEFECTENTRY_H

#include "dataEntry.h"

// Dialog class for adding new defect entries to the 'defects' table of the database
class DefectEntry : public DataEntry
{
    Q_OBJECT
public:
    explicit DefectEntry(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

private:
    virtual QList<QString> createLabels();
    virtual void setDBFields();
};

#endif // DEFECTENTRY_H
