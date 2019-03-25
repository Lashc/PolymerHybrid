#include "control.h"
#include "menu.h"
#include "databaseMenu.h"
#include <QStackedWidget>
#include <QVBoxLayout>

Control::Control(QWidget *parent) : QWidget(parent)
{
    // Create and add pages to a stacked widget
    pages = new QStackedWidget;
    Menu* mainMenu = new Menu(this);
    DatabaseMenu* databasePage = new DatabaseMenu(this);
    pages->addWidget(mainMenu);
    pages->addWidget(databasePage);

    // Connect signals and slots for changing pages via buttons
    connect(mainMenu, SIGNAL(buttonClicked(int)), pages, SLOT(setCurrentIndex(int)));

    // Layout for application
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(pages);
    setLayout(layout);
}

void Control::returnToMainMenu()
{
    pages->setCurrentIndex(0);
}
