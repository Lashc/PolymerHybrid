#ifndef TESTENTRY_H
#define TESTENTRY_H

#include "dataEntry.h"
#include <QLineEdit>

// Dialog class for adding new test entries to the 'tolerances' and 'tensile' tables of the database
class TestEntry : public DataEntry
{
    Q_OBJECT
public:
    explicit TestEntry(const QStringList&, QWidget* parent = nullptr);
    virtual QStringList getData() const;

private:
    QVector<QLineEdit *> toleranceLineEdits;
    QVector<QVector<QLineEdit *>> couponLineEdits;
};

#endif // TESTENTRY_H
