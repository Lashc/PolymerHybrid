#ifndef PRINTENTRY_H
#define PRINTENTRY_H

#include "dataEntry.h"

class QLineEdit;
class QTextEdit;

// Dialog class for adding new print entries to the 'prints' table of the database
class PrintEntry : public DataEntry
{
    Q_OBJECT
public:
    explicit PrintEntry(const QStringList&, QWidget* parent = nullptr);
    QStringList getData() const override;

private:
    QVector<QLineEdit *> lineEdits;
    QTextEdit* notes;
};

#endif // PRINTENTRY_H
