#include "printEntry.h"
#include <QGridLayout>

PrintEntry::PrintEntry(const QStringList& labels, QWidget* parent)
    : DataEntry(labels, parent)
{
    // Add labels and line edits to a grid layout
    QGridLayout* lineEditLayout = new QGridLayout;
    for (int i = 0; i < labelTexts.length(); i++) {
        lineEditLayout->addWidget(new QLabel(labelTexts[i]), i / 2, (2 * i) % 4);
        QLineEdit* input = new QLineEdit;
        lineEdits.append(input);
        lineEditLayout->addWidget(input, i / 2, ((2 * i) + 1) % 4);
    }

    // Add the grid layout to the main layout
    QVBoxLayout* mainLayout = reinterpret_cast<QVBoxLayout*>(layout());
    mainLayout->insertLayout(1, lineEditLayout);
}

QStringList PrintEntry::getData() const
{
    QStringList data;
    foreach(const QLineEdit* input, lineEdits)
        data.append(input->text());
    return data;
}
