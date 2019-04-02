#include "testEntry.h"
#include "databaseColumn.h"
#include "types.h"
#include <QLineEdit>
#include <QTabWidget>
#include <QLabel>
#include <QGridLayout>
#include <QMessageBox>

TestEntry::TestEntry(const QVector<DatabaseColumn*>& DBColumns, QWidget* parent)
    : DataEntry(DBColumns, parent)
{
    // Tab widget and list of tabs
    QTabWidget* tabs = new QTabWidget(this);
    QVector<QWidget *> tabList;

    // Create tolerances tab with labels and line edits in grid layout
    QWidget* toleranceTab = new QWidget;
    QGridLayout* toleranceLayout = new QGridLayout;
    const int numColumns = columns.length();
    const int numToleranceInputs = numColumns - NUM_TENSILE_TESTS;
    for (int i = 0; i < numToleranceInputs; i++) {
        const DatabaseColumn* column = columns[i];
        toleranceLayout->addWidget(new QLabel(column->label + ":"), i / 2, (2 * i) % 4);
        QLineEdit* input = new QLineEdit;
        if (column->validator)
            input->setValidator(column->validator);
        if (column->validatorType == "file")
            input->setMaxLength(column->validatorArgs[0].toInt());
        if (column->required)
            input->setStyleSheet("border: 2px solid rgb(201, 21, 58);"
                                 "border-radius: 2px;");
        toleranceLineEdits.append(input);
        toleranceLayout->addWidget(input, i / 2, ((2 * i) + 1) % 4);
    }
    toleranceTab->setLayout(toleranceLayout);
    tabs->addTab(toleranceTab, "Tolerances");

    // Create tensile test tabs for coupons with labels and line edits in grid layouts
    QVector<QString> tensileLabels;
    for (int i = numToleranceInputs; i < numColumns; i++)
        tensileLabels.append(columns[i]->label);
    for (int i = 0; i < NUM_COUPONS; i++) {
        QWidget* couponTab = new QWidget;
        QVector<QLineEdit *> lineEdits;
        QGridLayout* couponLayout = new QGridLayout;
        for (int j = 0; j < NUM_TENSILE_TESTS; j++) {
            const DatabaseColumn* column = columns[numToleranceInputs + j];
            couponLayout->addWidget(new QLabel(tensileLabels[j] + ":"), j / 2, (2 * j) % 4);
            QLineEdit* input = new QLineEdit;
            if (column->validator)
                input->setValidator(column->validator);
            if (column->required)
                input->setStyleSheet("border: 2px solid rgb(201, 21, 58);"
                                     "border-radius: 2px;");
            lineEdits.append(input);
            couponLayout->addWidget(input, j / 2, ((2 * j) + 1) % 4);
        }
        couponLineEdits.append(lineEdits);
        couponTab->setLayout(couponLayout);
        tabList.append(couponTab);
        tabs->addTab(couponTab, "Coupon " + QString::number(i + 1));
    }

    // Add the tab widget to the main layout
    QVBoxLayout* mainLayout = reinterpret_cast<QVBoxLayout*>(layout());
    mainLayout->insertWidget(1, tabs);
}

QStringList TestEntry::getData() const
{
    QStringList data;
    foreach(const QLineEdit* tolerInput, toleranceLineEdits)
        data.append(tolerInput->text());
    foreach(const QVector<QLineEdit*>& couponInputs, couponLineEdits) {
        QStringList tensileList;
        foreach(const QLineEdit* tensileInput, couponInputs)
            tensileList.append(tensileInput->text());
        data.append(tensileList.join(", "));
    }
    return data;
}

void TestEntry::validateData()
{
    const int numToleranceInputs = toleranceLineEdits.length();
    QVector<bool> validTolerances(numToleranceInputs, true);
    int numErrors = 0;
    for (int i = 0; i < numToleranceInputs; i++) {
        const QLineEdit* tolerInput = toleranceLineEdits[i];
        if ((columns[i]->required && tolerInput->text().isEmpty()) ||
            (!tolerInput->text().isEmpty() && !tolerInput->hasAcceptableInput())) {
                validTolerances[i] = false;
                numErrors++;
        }
    }

    const int numTensiles = couponLineEdits[0].length();
    const int numCoupons = couponLineEdits.length();
    const int tensileOffset = columns.length() - numTensiles;
    QVector<bool> validCoupons(numCoupons, true);
    for (int i = 0; i < numCoupons; i++) {
        for (int j = 0; j < numTensiles; j++) {
            const QLineEdit* tensileInput = couponLineEdits[i][j];
            if ((columns[tensileOffset + j]->required && tensileInput->text().isEmpty()) ||
                (!tensileInput->text().isEmpty() && !tensileInput->hasAcceptableInput()))
                    validCoupons[i] = false;
        }
        if (!validCoupons[i])
            numErrors++;
    }

    if (numErrors) {
        QString invalidInputs;
        for (int i = 0; i < numToleranceInputs; i++)
            if(!validTolerances[i])
                invalidInputs.append("* " + columns[i]->label + " has invalid input\n");
        for (int i = 0; i < numCoupons; i++)
            if (!validCoupons[i])
                invalidInputs.append("* Coupon " + QString::number(i + 1) + " has invalid input\n");
        invalidInputs.prepend(QString::number(numErrors) + (numErrors == 1 ? " error: \n\n" : " errors:\n\n"));
        QMessageBox errorDialog(QMessageBox::Warning, "Invalid input", invalidInputs, QMessageBox::Ok);
        errorDialog.exec();
    }
    else
        accept();
}
