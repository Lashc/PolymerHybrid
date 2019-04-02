#include "defectEntry.h"
#include "databaseColumn.h"
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QGridLayout>
#include <QMessageBox>

DefectEntry::DefectEntry(const QVector<DatabaseColumn*>& DBColumns, QWidget* parent)
    : DataEntry(DBColumns, parent)
{
    // Create labels, a line edit, and a text edit and add them to a grid layout
    printIDEdit = new QLineEdit;
    printIDEdit->setValidator(columns[0]->validator);
    printIDEdit->setStyleSheet("border: 2px solid rgb(201, 21, 58);"
                               "border-radius: 2px;");
    descEdit = new QTextEdit;
    descEdit->setStyleSheet("border: 2px solid rgb(201, 21, 58);"
                            "border-radius: 2px;");
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

void DefectEntry::validateData()
{
    int numErrors = 0;
    QString invalidInputs;
    if (!printIDEdit->hasAcceptableInput()) {
        numErrors++;
        invalidInputs.append("* " + columns[0]->label + " has invalid input\n");
    }
    const int maxDescLength = columns.last()->validatorArgs[0].toInt();
    if (descEdit->toPlainText().length() > maxDescLength || descEdit->toPlainText().isEmpty()) {
        numErrors++;
        invalidInputs.append("* " + columns.last()->label + " has invalid input\n");
    }
    if (numErrors) {
        invalidInputs.prepend(QString::number(numErrors) + (numErrors == 1 ? " error: \n\n" : " errors:\n\n"));
        QMessageBox errorDialog(QMessageBox::Warning, "Invalid input", invalidInputs, QMessageBox::Ok);
        errorDialog.exec();
    }
    else
        accept();
}
