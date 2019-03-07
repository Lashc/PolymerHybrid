#include "defectEntry.h"
#include <QGridLayout>

DefectEntry::DefectEntry(const QVector<QString>& fields, const QVector<QString>& labels, QWidget* parent)
    : DataEntry(fields, labels, parent)
{
    // Create labels, a line edit, and a text edit and add them to a grid layout
    printIDEdit = new QLineEdit;
    descEdit = new QTextEdit;
    QGridLayout* lineEditLayout = new QGridLayout;
    lineEditLayout->addWidget(new QLabel(labelTexts[0]), 0, 0);
    lineEditLayout->addWidget(printIDEdit, 0, 1);
    lineEditLayout->addWidget(new QLabel(labelTexts[1]), 1, 0);
    lineEditLayout->addWidget(descEdit, 1, 1);

    // Add the input layout to the main layout
    QVBoxLayout* mainLayout = reinterpret_cast<QVBoxLayout*>(layout());
    mainLayout->insertLayout(1, lineEditLayout);
}

QHash<QString, QString> DefectEntry::getData() const
{
    QHash<QString, QString> data;
    data.insert(dbFields[0], printIDEdit->text());
    data.insert(dbFields[1], descEdit->toPlainText());
    return data;
}
