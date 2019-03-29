#include "dataEntry.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

DataEntry::DataEntry(QStringList labels, QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f), fieldLabels(labels)
{
    // Add colons in front of the labels
    const int numLabels = labels.length();
    for (int i = 0; i < numLabels; i++)
        fieldLabels[i].append(":");

    // Title widget
    title = new QLabel("Enter data:");
    title->setFont(QFont("Futura", 20, QFont::Medium));

    // Action buttons for submitting and canceling database insertions
    QSize btnSize(100, 50);
    QFont btnFont("Gotham", 15, QFont::Medium);
    submitBtn = new QPushButton("Submit");
    submitBtn->setDefault(true);
    submitBtn->setMinimumSize(btnSize);
    submitBtn->setFont(btnFont);
    submitBtn->setObjectName("submit");
    cancelBtn = new QPushButton("Cancel");
    cancelBtn->setMinimumSize(btnSize);
    cancelBtn->setFont(btnFont);
    cancelBtn->setObjectName("cancel");

    // Action button layout
    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addWidget(submitBtn);
    btnLayout->addWidget(cancelBtn);

    // Main layout
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(title);
    layout->addLayout(btnLayout);
    setLayout(layout);

    // Connect signals and slots for action buttons
    connect(submitBtn, SIGNAL(released()), this, SLOT(accept()));
    connect(cancelBtn, SIGNAL(released()), this, SLOT(reject()));
}

DataEntry::~DataEntry()
{

}
