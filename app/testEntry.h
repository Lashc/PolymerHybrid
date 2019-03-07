#ifndef TESTENTRY_H
#define TESTENTRY_H

#include "dataEntry.h"
#include <QLineEdit>

class TestEntry : public DataEntry
{
    Q_OBJECT
public:
    explicit TestEntry(const QVector<QString>& fields, const QVector<QString>& labels, QWidget* parent = nullptr);
    virtual QHash<QString, QString> getData() const;

private:
    QVector<QLineEdit *> toleranceLineEdits;
    QVector<QVector<QLineEdit *>> couponLineEdits;
};

#endif // TESTENTRY_H
