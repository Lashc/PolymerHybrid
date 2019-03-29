#include "printEntry.h"
#include <QLineEdit>
#include <QTextEdit>
#include <QGridLayout>
#include <QLabel>

PrintEntry::PrintEntry(QStringList labels, QWidget* parent)
    : DataEntry(labels, parent)
{
    // Add labels and line edits to a grid layout
    QGridLayout* lineEditLayout = new QGridLayout;
    const int numLabels = fieldLabels.length();
    for (int i = 0; i < numLabels - 1; i++) {
        lineEditLayout->addWidget(new QLabel(fieldLabels[i]), i / 2, (2 * i) % 4);
        QLineEdit* input = new QLineEdit;
        lineEdits.append(input);
        lineEditLayout->addWidget(input, i / 2, ((2 * i) + 1) % 4);
    }

    // Layout for writing notes
    QHBoxLayout* notesLayout = new QHBoxLayout;
    notes = new QTextEdit;
    notesLayout->addWidget(new QLabel(fieldLabels[numLabels - 1]));
    notesLayout->addWidget(notes);
    notesLayout->setSpacing(30);

    // Add the grid layout to the main layout
    QVBoxLayout* mainLayout = reinterpret_cast<QVBoxLayout*>(layout());
    mainLayout->insertLayout(1, lineEditLayout);
    mainLayout->insertLayout(2, notesLayout);
}

QStringList PrintEntry::getData() const
{
    QStringList data;
    foreach(const QLineEdit* input, lineEdits)
        data.append(input->text());
    data.append(notes->toPlainText());
    return data;
}
