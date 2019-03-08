#ifndef DEFECTENTRY_H
#define DEFECTENTRY_H

#include "dataEntry.h"
#include <QLineEdit>
#include <QTextEdit>

// Dialog class for adding new defect entries to the 'defects' table of the database
class DefectEntry : public DataEntry
{
    Q_OBJECT
public:
    explicit DefectEntry(const QStringList&, QWidget* parent = nullptr);
    virtual QStringList getData() const;

private:
    QLineEdit* printIDEdit;
    QTextEdit* descEdit;
};

#endif // DEFECTENTRY_H
