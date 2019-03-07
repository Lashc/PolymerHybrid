#ifndef PRINTENTRY_H
#define PRINTENTRY_H

#include "dataEntry.h"
#include <QLineEdit>

// Dialog class for adding new print entries to the 'prints' table of the database
class PrintEntry : public DataEntry
{
    Q_OBJECT
public:
    explicit PrintEntry(const QVector<QString>&, const QVector<QString>&, QWidget* parent = nullptr);
    virtual QHash<QString, QString> getData() const;

private:
    QVector<QLineEdit *> lineEdits;
};

#endif // PRINTENTRY_H
