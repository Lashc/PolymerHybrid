#include "menu.h"
#include "types.h"
#include <QLabel>
#include <QButtonGroup>
#include <QPushButton>
#include <QGridLayout>

Menu::Menu(QWidget *parent) : QWidget(parent)
{
    // Button properties
    QSizePolicy btnPolicy(QSizePolicy::Minimum, QSizePolicy::Preferred, QSizePolicy::PushButton);
    QFont btnFont("Gotham", 25, QFont::Medium);
    QSize maxBtnSize(350, 175);
    QSize minBtnSize(150, 75);

    // Create the database, camera, and operations manual buttons
    databaseBtn = new QPushButton("Database");
    databaseBtn->setFont(btnFont);
    databaseBtn->setMaximumSize(maxBtnSize);
    databaseBtn->setMinimumSize(minBtnSize);
    databaseBtn->setSizePolicy(btnPolicy);

    cameraBtn = new QPushButton("Camera");
    cameraBtn->setFont(btnFont);
    cameraBtn->setMaximumSize(maxBtnSize);
    cameraBtn->setMinimumSize(minBtnSize);
    cameraBtn->setSizePolicy(btnPolicy);

    manualBtn = new QPushButton("Operations Manual");
    manualBtn->setFont(btnFont);
    manualBtn->setMaximumSize(maxBtnSize);
    manualBtn->setMinimumSize(minBtnSize);
    manualBtn->setSizePolicy(btnPolicy);

    btnGroup = new QButtonGroup(this);
    btnGroup->addButton(databaseBtn, databaseID);
    btnGroup->addButton(cameraBtn, cameraID);
    btnGroup->addButton(manualBtn, manualID);

    // Horizontal layout for buttons
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(databaseBtn);
    btnLayout->addWidget(cameraBtn);
    btnLayout->addWidget(manualBtn);
    btnLayout->setSpacing(30);

    // Create the label at the top
    title = new QLabel("Polymer Hybrid Manufacturing");
    title->setObjectName("title");
    title->setAlignment(Qt::AlignCenter);
    title->setMinimumHeight(150);
    title->setWordWrap(true);
    title->setFont(QFont("Futura", 50));

    // Create the banner at the bottom
    banner = new QLabel("LSU College of Engineering");
    banner->setObjectName("banner");
    banner->setFont(QFont("Arial", 30, QFont::Bold));
    banner->setAlignment(Qt::AlignCenter);
    banner->setMaximumHeight(200);
    banner->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    banner->setScaledContents(true);
    banner->setMargin(10);

    // Create a grid layout and add the widgets and layout to it
    QGridLayout* grid = new QGridLayout(this);
    grid->addWidget(title, 0, 0, 1, 2);
    grid->addLayout(btnLayout, 1, 0, 1, 2);
    grid->addWidget(banner, 2, 0, 1, 2);
    grid->setVerticalSpacing(100);
    grid->setHorizontalSpacing(20);
    setLayout(grid);

    // Notify the main 'Control' widget of a button click (and release)
    connect(btnGroup, SIGNAL(buttonReleased(int)), this, SLOT(onButtonReleased(int)));
}

void Menu::onButtonReleased(int id)
{
    emit buttonReleased(id);
}
