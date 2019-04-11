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
    explicit PrintEntry(const QVector<DatabaseColumn*>&, QWidget* parent = nullptr);
    explicit PrintEntry(const QVector<DatabaseColumn*>&, QStringList data, QWidget* parent = nullptr);
    QStringList getData() const override;

private:
    QVector<QLineEdit *> lineEdits;
    QTextEdit* notes;

public slots:
    void validateData() override;
};

#endif // PRINTENTRY_H
