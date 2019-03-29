#include "defectEntry.h"
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QGridLayout>

DefectEntry::DefectEntry(QStringList labels, QWidget* parent)
    : DataEntry(labels, parent)
{
    // Create labels, a line edit, and a text edit and add them to a grid layout
    printIDEdit = new QLineEdit;
    descEdit = new QTextEdit;
    QGridLayout* lineEditLayout = new QGridLayout;
    lineEditLayout->addWidget(new QLabel(fieldLabels[0]), 0, 0);
    lineEditLayout->addWidget(printIDEdit, 0, 1);
    lineEditLayout->addWidget(new QLabel(fieldLabels[1]), 1, 0);
    lineEditLayout->addWidget(descEdit, 1, 1);

    // Add the input layout to the main layout
    QVBoxLayout* mainLayout = reinterpret_cast<QVBoxLayout*>(layout());
    mainLayout->insertLayout(1, lineEditLayout);
}

QStringList DefectEntry::getData() const
{
    return QStringList( { printIDEdit->text(), descEdit->toPlainText() } );
}
