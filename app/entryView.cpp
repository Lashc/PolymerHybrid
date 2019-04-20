#include "entryView.h"
#include "types.h"
#include <QSqlRecord>
#include <QSqlField>
#include <QVBoxLayout>
#include <QLabel>
#include <QVariant>
#include <QTextEdit>

EntryView::EntryView(int dataID, int rowNum, QVector<QString> labels, const QSqlRecord& row, QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
{
    // Main layout for dialog with title
    QVBoxLayout* layout = new QVBoxLayout;
    setLayout(layout);
    layout->setSpacing(20);
    QLabel* header = new QLabel("Row " + QString::number(rowNum + 1) + " data");
    QFont headerFont("Futura", 24, QFont::Medium);
    headerFont.setUnderline(true);
    header->setFont(headerFont);
    header->setStyleSheet("QLabel { color: rgb(201, 21, 58); }");
    layout->addWidget(header);

    // Get the values of the fields
    QVector<QString> fields;
    for (int i = 1, rowLength = row.count(); i < rowLength; i++) {
        QVariant value = row.field(i).value();
        fields.append(value.toString());
    }
    const int numFields = fields.length();

    // Grid layout with pairs of labels and values
    QGridLayout* gridLayout = new QGridLayout;
    layout->addLayout(gridLayout);
    gridLayout->setHorizontalSpacing(20);
    gridLayout->setVerticalSpacing(10);
    QFont fieldLabelFont("Avenir", 18, QFont::Bold);
    QString fieldStyle("border: 3px solid rgb(0, 135, 200);"
                       "border-radius: 4px; padding: 5px;"
                       "background-color: rgb(230, 230, 230);"
                       "font: normal normal 16px \"Gotham\";");
    const int numLabels = labels.size();
    int i;
    for (i = 0; i < numLabels - 1; i++) {
        QLabel* fieldLabel = new QLabel(labels[i]);
        fieldLabel->setFont(fieldLabelFont);
        gridLayout->addWidget(fieldLabel, i / 2, (2 * i) % 4);
        QLabel* fieldText = new QLabel(fields[i]);
        fieldText->setStyleSheet("QLabel { " + fieldStyle + " }");
        fieldText->setAlignment(Qt::AlignCenter);
        fieldText->setTextInteractionFlags(Qt::TextSelectableByMouse);
        fieldText->setWordWrap(true);
        fieldText->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
        gridLayout->addWidget(fieldText, i / 2, ((2 * i) + 1) % 4);
    }

    // Add the last widget to the dialog, which varies by data category
    if (dataID == printID || dataID == defectID) {
        QHBoxLayout* notesLayout = new QHBoxLayout;
        QLabel* notesLabel = new QLabel(labels[numLabels - 1]);
        notesLabel->setFont(fieldLabelFont);
        notesLayout->addWidget(notesLabel, 1);
        QTextEdit* notesText = new QTextEdit;
        notesText->append(fields[numFields - 1]);
        notesText->setStyleSheet("QTextEdit { " + fieldStyle + " }");
        notesText->setTextInteractionFlags(Qt::TextSelectableByMouse);
        notesText->setFixedHeight(100);
        notesLayout->addWidget(notesText, 5);
        layout->addLayout(notesLayout);
    }
    else {
        QLabel* fieldLabel = new QLabel(labels[i]);
        fieldLabel->setFont(fieldLabelFont);
        gridLayout->addWidget(fieldLabel, i / 2, (2 * i) % 4);
        QLabel* fieldText = new QLabel(fields[i]);
        fieldText->setStyleSheet(fieldStyle);
        fieldText->setAlignment(Qt::AlignCenter);
        fieldText->setTextInteractionFlags(Qt::TextSelectableByMouse);
        fieldText->setWordWrap(true);
        gridLayout->addWidget(fieldText, i / 2, ((2 * i) + 1) % 4);
    }
}
