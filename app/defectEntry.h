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
    explicit DefectEntry(const QVector<DatabaseColumn*>&, bool, QWidget* parent = nullptr);
    explicit DefectEntry(const QVector<DatabaseColumn*>&, QStringList, bool, QWidget* parent = nullptr);
    QStringList getData() const override;

private:
    QLineEdit* printIDEdit;
    QTextEdit* descEdit;

public slots:
    void validateData() override;
};

#endif // DEFECTENTRY_H
