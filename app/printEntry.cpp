#include "printEntry.h"
#include "databaseColumn.h"
#include <QLineEdit>
#include <QTextEdit>
#include <QGridLayout>
#include <QLabel>

PrintEntry::PrintEntry(const QVector<DatabaseColumn*>& DBColumns, QWidget* parent)
    : DataEntry(DBColumns, parent)
{
    // Add labels and line edits to a grid layout
    QGridLayout* lineEditLayout = new QGridLayout;
    const int numColumns = columns.length();
    for (int i = 0; i < numColumns - 1; i++) {
        const DatabaseColumn* column = columns[i];
        lineEditLayout->addWidget(new QLabel(column->label + ":"), i / 2, (2 * i) % 4);
        QLineEdit* input = new QLineEdit;
        if (column->validator)
            input->setValidator(column->validator);
        lineEdits.append(input);
        lineEditLayout->addWidget(input, i / 2, ((2 * i) + 1) % 4);
    }

    // Layout for writing notes
    QHBoxLayout* notesLayout = new QHBoxLayout;
    notes = new QTextEdit;
    notesLayout->addWidget(new QLabel(columns[numColumns - 1]->label + ":"));
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
    foreach(const QLineEdit* input, lineEdits) {
        if (!input->hasAcceptableInput() && !input->text().isEmpty())
            return QStringList();
        else
            data.append(input->text());
    }
    data.append(notes->toPlainText());
    return data;
}
