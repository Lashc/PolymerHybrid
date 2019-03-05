#include "defectEntry.h"

DefectEntry::DefectEntry(QWidget *parent, Qt::WindowFlags f) : DataEntry(parent, f)
{
    // Set list of database table fields
    setDBFields();

    // Set labels
    QList<QString> labels = createLabels();
    for (int i = 0; i < labels.length(); i++) {
        labelList.append(new QLabel(labels[i]));
        lineEditList.append(new QLineEdit);
    }

    // Add the labels and line edits to the grid layout
    QGridLayout* lineEditLayout = dynamic_cast<QGridLayout *>(layout()->itemAt(1));
    for (int i = 0; i < labelList.length(); i++) {
        lineEditLayout->addWidget(labelList[i], i / 2, (2 * i) % 4);
        lineEditLayout->addWidget(lineEditList[i], i / 2, ((2 * i) + 1) % 4);
    }
}

QList<QString> DefectEntry::createLabels()
{
    QList<QString> labels = { "Print ID", "Description" };
    return labels;
}

void DefectEntry::setDBFields()
{
    dbFields = { "print_id", "description" };
}
