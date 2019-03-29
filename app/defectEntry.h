#ifndef DEFECTENTRY_H
#define DEFECTENTRY_H

#include "dataEntry.h"

class QLineEdit;
class QTextEdit;

// Dialog class for adding new defect entries to the 'defects' table of the database
class DefectEntry : public DataEntry
{
    Q_OBJECT
public:
    explicit DefectEntry(QStringList, QWidget* parent = nullptr);
    QStringList getData() const override;

private:
    QLineEdit* printIDEdit;
    QTextEdit* descEdit;
};

#endif // DEFECTENTRY_H
