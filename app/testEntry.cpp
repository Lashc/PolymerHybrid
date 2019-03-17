#include "testEntry.h"
#include "types.h"
#include <QLineEdit>
#include <QTabWidget>
#include <QLabel>
#include <QGridLayout>

TestEntry::TestEntry(const QStringList& labels, QWidget* parent)
    : DataEntry(labels, parent)
{
    // Tab widget and list of tabs
    QTabWidget* tabs = new QTabWidget(this);
    QVector<QWidget *> tabList;

    // Create tolerances tab with labels and line edits in grid layout
    QWidget* toleranceTab = new QWidget;
    QGridLayout* toleranceLayout = new QGridLayout;
    int numToleranceInputs = labels.length() - NUM_TENSILE_TESTS;
    for (int i = 0; i < numToleranceInputs; i++) {
        toleranceLayout->addWidget(new QLabel(labels[i]), i / 2, (2 * i) % 4);
        QLineEdit* input = new QLineEdit;
        toleranceLineEdits.append(input);
        toleranceLayout->addWidget(input, i / 2, ((2 * i) + 1) % 4);
    }
    toleranceTab->setLayout(toleranceLayout);
    tabs->addTab(toleranceTab, "Tolerances");

    // Create tensile test tabs for coupons with labels and line edits in grid layouts
    QVector<QString> tensileLabels;
    for (int i = numToleranceInputs; i < labels.length(); i++)
        tensileLabels.append(labels[i]);
    for (int i = 0; i < NUM_COUPONS; i++) {
        QWidget* couponTab = new QWidget;
        QVector<QLineEdit *> lineEdits;
        QGridLayout* couponLayout = new QGridLayout;
        for (int i = 0; i < NUM_TENSILE_TESTS; i++) {
            couponLayout->addWidget(new QLabel(tensileLabels[i]), i / 2, (2 * i) % 4);
            QLineEdit* input = new QLineEdit;
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
    foreach(const QLineEdit* tolInput, toleranceLineEdits)
        data.append(tolInput->text());
    foreach(const QVector<QLineEdit *>& couponTensileList, couponLineEdits) {
        QStringList tensileList;
        foreach(const QLineEdit* tensileInputs, couponTensileList)
            tensileList.append(tensileInputs->text());
        data.append(tensileList.join(", "));
    }
    return data;
}
