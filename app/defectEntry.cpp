#include "defectEntry.h"
#include "databaseColumn.h"
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QGridLayout>

DefectEntry::DefectEntry(const QVector<DatabaseColumn*>& DBColumns, QWidget* parent)
    : DataEntry(DBColumns, parent)
{
    // Create labels, a line edit, and a text edit and add them to a grid layout
    printIDEdit = new QLineEdit;
    if (columns[0]->validator)
        printIDEdit->setValidator(columns[0]->validator);
    descEdit = new QTextEdit;
    QGridLayout* lineEditLayout = new QGridLayout;
    lineEditLayout->addWidget(new QLabel(columns[0]->label + ":"), 0, 0);
    lineEditLayout->addWidget(printIDEdit, 0, 1);
    lineEditLayout->addWidget(new QLabel(columns[1]->label + ":"), 1, 0);
    lineEditLayout->addWidget(descEdit, 1, 1);

    // Add the input layout to the main layout
    QVBoxLayout* mainLayout = reinterpret_cast<QVBoxLayout*>(layout());
    mainLayout->insertLayout(1, lineEditLayout);
}

QStringList DefectEntry::getData() const
{
    return { printIDEdit->text(), descEdit->toPlainText() };
}
