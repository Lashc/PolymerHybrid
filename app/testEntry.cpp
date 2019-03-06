#include "testEntry.h"
#include <QTabWidget>
#include <QGridLayout>

const int NUM_COUPONS = 12;
const int NUM_TENSILE_TESTS = 5;

TestEntry::TestEntry(QList<QString> fields, QList<QString> labels, QWidget* parent)
    : DataEntry(fields, labels, parent)
{
    // Remove 'coupon' field from 'tolerances' table and ID and tolerance ID fields from 'tensile' table
    dbFields.removeOne("coupon");
    dbFields.removeOne("id");
    dbFields.removeOne("tolerance_id");

    // Tab widget and list of tabs
    QTabWidget* tabs = new QTabWidget(this);
    QList<QWidget *> tabList;

    // Create tolerances tab with labels and line edits in grid layout
    QWidget* toleranceTab = new QWidget;
    QGridLayout* toleranceLayout = new QGridLayout;
    for (int i = 0; i < 4; i++) {
        toleranceLayout->addWidget(new QLabel(labelTexts[i]), i / 2, (2 * i) % 4);
        QLineEdit* input = new QLineEdit;
        toleranceLineEdits.append(input);
        toleranceLayout->addWidget(input, i / 2, ((2 * i) + 1) % 4);
    }
    toleranceTab->setLayout(toleranceLayout);
    tabs->addTab(toleranceTab, "Tolerances");

    // Create tensile test tabs for coupons with labels and line edits in grid layouts
    QList<QString> tensileLabels;
    for (int i = labelTexts.length() - NUM_TENSILE_TESTS; i < labelTexts.length(); i++)
        tensileLabels.append(labelTexts[i]);
    for (int i = 0; i < NUM_COUPONS; i++) {
        QWidget* couponTab = new QWidget;
        QList<QLineEdit *> lineEdits;
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

QHash<QString, QString> TestEntry::getData() const
{
    QHash<QString, QString> data;
    int i;
    for (i = 0; i < toleranceLineEdits.length(); i++)
        data.insert(dbFields[i], toleranceLineEdits[i]->text());
    for (int tensile = 0; tensile < NUM_TENSILE_TESTS; tensile++) {
        QStringList tensileList;
        for (int coupon = 0; coupon < NUM_COUPONS; coupon++)
            tensileList.append(couponLineEdits[coupon][tensile]->text());
        data.insert(dbFields[i + tensile], tensileList.join(", "));
    }
    return data;
}
