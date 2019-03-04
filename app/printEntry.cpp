#include "printEntry.h"

PrintEntry::PrintEntry(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
    // Main dialog layout
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setSpacing(20);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    // Title widget
    QLabel* title = new QLabel("Enter Information:");
    title->setFont(QFont("Futura", 20, QFont::Medium));
    layout->addWidget(title, 1);

    // Line edit layout
    QGridLayout* lineEditLayout = new QGridLayout;
    layout->addLayout(lineEditLayout);
    initLineEdits(lineEditLayout);
    lineEditLayout->setSpacing(20);

    // Horizontal buttons and associated layout
    QHBoxLayout* btnLayout = new QHBoxLayout;
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
    btnLayout->addWidget(submitBtn);
    btnLayout->addWidget(cancelBtn);
    layout->addLayout(btnLayout, 2);

    setLayout(layout);

    // Connect signals and slots
    connect(submitBtn, SIGNAL(released()), this, SLOT(populateMap()));
    connect(submitBtn, SIGNAL(released()), this, SLOT(accept()));
    connect(cancelBtn, SIGNAL(released()), this, SLOT(reject()));
}

QList<QString> PrintEntry::getData() const
{
    return data;
}

void PrintEntry::initLineEdits(QGridLayout* gridLayout)
{
    inputLabels = { "Date (YYYY-MM-DD):", "Description:", "Experiment:", "Drying time (hr):",
                    "Setup time (min):", "Cycle time (hr):", "Shut down time (min):", "Transition time (hr):",
                    "Nozzle temperature (°C):", "Spindle speeed (RPM):", "Feed rate (m/min):",
                    "Bed temperature (°C):", "Dryer temperature (°C):", "Dryer method:",
                    "Surface finish:", "Layer time (min):", "Rapid %:", "Thermal video:", "Visual video:" };
    for (int i = 0; i < inputLabels.size(); i++) {
        QLineEdit* input = new QLineEdit;
        QLabel* label = new QLabel(inputLabels[i]);
        inputList.append(input);
        gridLayout->addWidget(label, i / 2, (2 * i) % 4);
        gridLayout->addWidget(input, i / 2, ((2 * i) + 1) % 4);
    }
}

void PrintEntry::populateMap()
{
    for (QLineEdit* input : inputList) {
        data.append(input->text());
    }
}
