#ifndef TESTENTRY_H
#define TESTENTRY_H

#include "dataEntry.h"
#include <QLineEdit>

class TestEntry : public DataEntry
{
    Q_OBJECT
public:
    explicit TestEntry(QList<QString> fields, QList<QString> labels, QWidget* parent = nullptr);
    virtual QHash<QString, QString> getData() const;

private:
    QList<QLineEdit *> toleranceLineEdits;
    QList<QList<QLineEdit *>> couponLineEdits;
};

#endif // TESTENTRY_H
