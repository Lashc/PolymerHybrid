#include "printEntry.h"
#include "databaseColumn.h"
#include <QLineEdit>
#include <QTextEdit>
#include <QGridLayout>
#include <QLabel>
#include <QValidator>
#include <QMessageBox>

PrintEntry::PrintEntry(const QVector<DatabaseColumn*>& DBColumns, QWidget* parent)
    : DataEntry(DBColumns, parent)
{
    // Add labels and line edits to a grid layout
    QGridLayout* lineEditLayout = new QGridLayout;
    const int numColumns = columns.length();
    for (int i = 0; i < numColumns - 1; i++) {
        const DatabaseColumn* column = columns[i];
        QLabel* inputLabel = new QLabel(column->label + ":");
        inputLabel->setFont(QFont("Gotham", 18));
        lineEditLayout->addWidget(inputLabel, i / 2, (2 * i) % 4);
        QLineEdit* input = new QLineEdit;
        if (column->validator)
            input->setValidator(column->validator);
        if (column->validatorType == "text" || column->validatorType == "file")
            input->setMaxLength(column->validatorArgs[0].toInt());
        if (column->required)
            input->setStyleSheet("border: 2px solid rgb(201, 21, 58);"
                                 "border-radius: 2px;");
        lineEdits.append(input);
        lineEditLayout->addWidget(input, i / 2, ((2 * i) + 1) % 4);
    }
    lineEditLayout->setVerticalSpacing(15);

    // Layout for writing notes
    QHBoxLayout* notesLayout = new QHBoxLayout;
    notes = new QTextEdit;
    QLabel* notesLabel = new QLabel(columns.last()->label + ":");
    notesLabel->setFont(QFont("Gotham", 18));
    notesLayout->addWidget(notesLabel);
    notesLayout->addWidget(notes);
    notesLayout->setSpacing(30);

    // Add the grid layout to the main layout
    QVBoxLayout* mainLayout = reinterpret_cast<QVBoxLayout*>(layout());
    mainLayout->insertLayout(1, lineEditLayout);
    mainLayout->insertLayout(2, notesLayout);
}

PrintEntry::PrintEntry(const QVector<DatabaseColumn*>& DBColumns, QStringList data, QWidget* parent)
    : PrintEntry(DBColumns, parent)
{
    const int numData = data.length();
    for (int i = 0; i < numData - 1; i++)
        lineEdits[i]->setText(data[i]);
    notes->setText(data.last());
}

QStringList PrintEntry::getData() const
{
    QStringList data;
    foreach(const QLineEdit* input, lineEdits)
        data.append(input->text());
    data.append(notes->toPlainText());
    return data;
}

void PrintEntry::validateData()
{
    const int numLineEdits = lineEdits.size();
    QVector<bool> valid(numLineEdits + 1, true);
    int numErrors = 0;
    for (int i = 0; i < numLineEdits; i++) {
        const QLineEdit* input = lineEdits[i];
        if ((columns[i]->required && input->text().isEmpty()) ||
            (!input->text().isEmpty() && !input->hasAcceptableInput())) {
                valid[i] = false;
                numErrors++;
        }
    }
    const int maxNotesLength = columns.last()->validatorArgs[0].toInt();
    if (notes->toPlainText().length() > maxNotesLength)
        valid.last() = false;
    if (numErrors) {
        QString invalidInputs;
        for (int i = 0; i < numLineEdits + 1; i++)
            if(!valid[i])
                invalidInputs.append("* " + columns[i]->label + " has invalid input\n");
        invalidInputs.prepend(QString::number(numErrors) + (numErrors == 1 ? " error: \n\n" : " errors:\n\n"));
        QMessageBox errorDialog(QMessageBox::Warning, "Invalid input", invalidInputs, QMessageBox::Ok);
        errorDialog.exec();
    }
    else
        accept();
}
