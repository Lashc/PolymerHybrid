#include "dataEntry.h"

DataEntry::DataEntry(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
    // Title widget
    title = new QLabel("Enter Information:");
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

    // Set up the layout
    configureLayout();

    // Connect signals and slots for action buttons
    connect(submitBtn, SIGNAL(released()), this, SLOT(accept()));
    connect(cancelBtn, SIGNAL(released()), this, SLOT(reject()));
}

DataEntry::~DataEntry()
{

}

QHash<QString, QString> DataEntry::getData() const
{
    QHash<QString, QString> data;
    for (int i = 0; i < dbFields.length(); i++)
        data.insert(dbFields[i], lineEditList[i]->text());
    return data;
}

void DataEntry::configureLayout()
{
    // Main dialog layout
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setSpacing(20);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    layout->addWidget(title, 1);

    // Line edit layout
    QGridLayout* lineEditLayout = new QGridLayout;
    layout->addLayout(lineEditLayout);
    lineEditLayout->setSpacing(20);

    // Action button layout
    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addWidget(submitBtn);
    btnLayout->addWidget(cancelBtn);
    layout->addLayout(btnLayout, 2);

    setLayout(layout);
}
