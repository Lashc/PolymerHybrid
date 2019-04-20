#include "defectEntry.h"
#include "databaseColumn.h"
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QGridLayout>
#include <QMessageBox>

DefectEntry::DefectEntry(const QVector<DatabaseColumn*>& DBColumns, bool isInsert, QWidget* parent)
    : DataEntry(DBColumns, parent)
{
    // Create labels, a line edit, and a text edit and add them to a grid layout
    printIDEdit = new QLineEdit;
    // Don't check a foreign key on update or let the user update it
    if (isInsert)
        printIDEdit->setValidator(columns[0]->validator);
    else
        printIDEdit->setReadOnly(true);
    printIDEdit->setStyleSheet("border: 2px solid rgb(201, 21, 58);"
                               "border-radius: 2px;");
    descEdit = new QTextEdit;
    descEdit->setStyleSheet("border: 2px solid rgb(201, 21, 58);"
                            "border-radius: 2px;");
    QGridLayout* lineEditLayout = new QGridLayout;
    QLabel* printLabel = new QLabel(columns[0]->label + ":");
    printLabel->setFont(QFont("Gotham", 18));
    lineEditLayout->addWidget(printLabel, 0, 0);
    lineEditLayout->addWidget(printIDEdit, 0, 1);
    QLabel* descLabel = new QLabel(columns[1]->label + ":");
    descLabel->setFont(QFont("Gotham", 18));
    lineEditLayout->addWidget(descLabel, 1, 0);
    lineEditLayout->addWidget(descEdit, 1, 1);

    // Add the input layout to the main layout
    QVBoxLayout* mainLayout = reinterpret_cast<QVBoxLayout*>(layout());
    mainLayout->insertLayout(1, lineEditLayout);
}

DefectEntry::DefectEntry(const QVector<DatabaseColumn*>& DBColumns, QStringList data, bool isInsert, QWidget* parent)
    : DefectEntry(DBColumns, isInsert, parent)
{
    printIDEdit->setText(data[0]);
    descEdit->setText(data.last());
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
