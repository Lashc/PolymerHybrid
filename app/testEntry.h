#ifndef TESTENTRY_H
#define TESTENTRY_H

#include "dataEntry.h"

class QLineEdit;

// Dialog class for adding new test entries to the 'tolerances' and 'tensile' tables of the database
class TestEntry : public DataEntry
{
    Q_OBJECT
public:
    explicit TestEntry(const QVector<DatabaseColumn*>&, bool, QWidget* parent = nullptr);
    explicit TestEntry(const QVector<DatabaseColumn*>&, QStringList, bool, QWidget* parent = nullptr);
    QStringList getData() const override;

private:
    QVector<QLineEdit *> toleranceLineEdits;
    QVector<QVector<QLineEdit *>> couponLineEdits;

public slots:
    void validateData() override;
};

#endif // TESTENTRY_H
