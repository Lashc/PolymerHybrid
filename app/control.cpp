#include "control.h"
#include "menu.h"
#include "types.h"
#include "databaseMenu.h"
#include "cameraMenu.h"
#include <cstdlib>
#include <string>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QDir>
#include <QMessageBox>

Control::Control(QWidget *parent) : QWidget(parent)
{
    // Create and add pages to a stacked widget
    pages = new QStackedWidget;
    Menu* mainMenu = new Menu(this);
    DatabaseMenu* databaseMenu = new DatabaseMenu(this);
    CameraMenu* cameraMenu = new CameraMenu(this);
    pages->addWidget(mainMenu);
    pages->addWidget(databaseMenu);
    pages->addWidget(cameraMenu);

    // Connect signals and slots for changing pages via buttons
    connect(mainMenu, SIGNAL(buttonReleased(int)), this, SLOT(onMenuButtonReleased(int)));

    // Layout for application
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(pages);
    layout->setMargin(5);
    setLayout(layout);
}

void Control::onMenuButtonReleased(int id)
{
    // Switch to the database or camera page
    if (id == databaseID || id == cameraID)
        pages->setCurrentIndex(id);

    // Open the operations manual PDF
    else if (id == manualID) {
        QString manualFilePath = QDir::homePath() + "/Documents/'Polymer Hybrid Manufacturing Operations Manual.pdf'";
        QString manualFileCheck = manualFilePath;
        if (!QFile::exists(manualFileCheck.remove('\''))) {
            QMessageBox PDFMissingDialog(QMessageBox::Warning, "No PDF Found",
                                         "The file \"Polymer Hybrid Manufacturing Operations Manual.pdf\" "
                                         "could not be found in the \"Documents\" folder!", QMessageBox::Ok, this);
            PDFMissingDialog.exec();
            return;
        }

        QString openManualCommand;
        #ifdef __linux__
            openManualCommand = ("xdg-open " + manualFilePath);
        #elif __APPLE__
            openManualCommand = ("open " + manualFilePath);
        #endif
        system(openManualCommand.toStdString().c_str());
    }
}

void Control::returnToMainMenu()
{
    pages->setCurrentIndex(0);
}
