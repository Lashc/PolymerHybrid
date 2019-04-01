#include "testEntry.h"
#include "databaseColumn.h"
#include "types.h"
#include <QLineEdit>
#include <QTabWidget>
#include <QLabel>
#include <QGridLayout>

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
        toleranceLineEdits.append(input);
        toleranceLayout->addWidget(input, i / 2, ((2 * i) + 1) % 4);
    }
    toleranceTab->setLayout(toleranceLayout);
    tabs->addTab(toleranceTab, "Tolerances");

    // Create tensile test tabs for coupons with labels and line edits in grid layouts
    QVector<QString> tensileLabels;
    QVector<QValidator*> tensileValidators;
    for (int i = numToleranceInputs; i < numColumns; i++) {
        tensileLabels.append(columns[i]->label);
        tensileValidators.append(columns[i]->validator);
    }
    for (int i = 0; i < NUM_COUPONS; i++) {
        QWidget* couponTab = new QWidget;
        QVector<QLineEdit *> lineEdits;
        QGridLayout* couponLayout = new QGridLayout;
        for (int i = 0; i < NUM_TENSILE_TESTS; i++) {
            couponLayout->addWidget(new QLabel(tensileLabels[i] + ":"), i / 2, (2 * i) % 4);
            QLineEdit* input = new QLineEdit;
            if (tensileValidators[i])
                input->setValidator(tensileValidators[i]);
            lineEdits.append(input);
            couponLayout->addWidget(input, i / 2, ((2 * i) + 1) % 4);
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
    foreach(const QLineEdit* tolerInput, toleranceLineEdits) {
        if (tolerInput->hasAcceptableInput())
            data.append(tolerInput->text());
        else
            return QStringList();
    }
    foreach(const QVector<QLineEdit *>& couponTensileList, couponLineEdits) {
        QStringList tensileList;
        foreach(const QLineEdit* tensileInput, couponTensileList)
            data.append(tensileInput->text());
        data.append(tensileList.join(", "));
    }
    return data;
}
