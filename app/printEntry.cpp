#include "printEntry.h"

PrintEntry::PrintEntry(QWidget *parent, Qt::WindowFlags f) : DataEntry(parent, f)
{
    // Set list of database table fields
    setDBFields();

    // Set labels
    QList<QString> labels = createLabels();
    for (int i = 0; i < labels.length(); i++) {
        labelList.append(new QLabel(labels[i]));
        lineEditList.append(new QLineEdit);
    }

    // Add the labels and line edits to the grid layout
    QGridLayout* lineEditLayout = dynamic_cast<QGridLayout *>(layout()->itemAt(1));
    for (int i = 0; i < labelList.length(); i++) {
        lineEditLayout->addWidget(labelList[i], i / 2, (2 * i) % 4);
        lineEditLayout->addWidget(lineEditList[i], i / 2, ((2 * i) + 1) % 4);
    }
}

QList<QString> PrintEntry::createLabels()
{
    QList<QString> labels = { "Date (YYYY-MM-DD):", "Description:", "Experiment:", "Drying time (hr):",
                              "Setup time (min):", "Cycle time (hr):", "Shut down time (min):", "Transition time (hr):",
                              "Nozzle temperature (°C):", "Spindle speeed (RPM):", "Feed rate (m/min):",
                              "Bed temperature (°C):", "Dryer temperature (°C):", "Dryer method:",
                              "Surface finish:", "Layer time (min):", "Rapid %:", "Thermal video:", "Visual video:" };
    return labels;
}

void PrintEntry::setDBFields()
{
    dbFields = { "date", "description", "experiment", "dry_time", "setup_time", "cycle_time", "shutdown_time",
                 "transition_time", "nozzle_temp", "spindle_speed", "feed_rate", "bed_temp", "dryer_temp",
                 "dryer_method", "surface_finish", "layer_time", "rapids", "thermal_video", "visual_video" };
}
