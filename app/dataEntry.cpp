#include "dataEntry.h"
#include <QVBoxLayout>

DataEntry::DataEntry(const QVector<QString>& fields, const QVector<QString>& labels, QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f), dbFields(fields), labelTexts(labels)
{
    // Title widget
    title = new QLabel("Enter information:");
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

    // Remove ID items of labels and add ':' in front of each label
    labelTexts.removeFirst();
    QVector<QString>::iterator i;
    for (i = labelTexts.begin(); i != labelTexts.end(); i++)
        i->append(":");

    // Connect signals and slots for action buttons
    connect(submitBtn, SIGNAL(released()), this, SLOT(accept()));
    connect(cancelBtn, SIGNAL(released()), this, SLOT(reject()));
}

DataEntry::~DataEntry()
{

}
