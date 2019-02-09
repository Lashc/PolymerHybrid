#include "menu.h"
#include <QGridLayout>
#include <QFile>

Menu::Menu(QWidget *parent) : QWidget(parent)
{
    // Set the stylesheet
    QFile qssFile(":/menu_style.qss");
    qssFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(qssFile.readAll());
    setStyleSheet(styleSheet);

    // Button properties
    QSizePolicy btnPolicy(QSizePolicy::Minimum, QSizePolicy::Minimum, QSizePolicy::PushButton);
    QFont btnFont("Gotham", 25, QFont::Medium);
    QSize btnSize(400, 200);

    // Create the database and camera buttons
    databaseBtn = new QPushButton("&Database");
    databaseBtn->setFont(btnFont);
    databaseBtn->setMaximumSize(btnSize);
    databaseBtn->setSizePolicy(btnPolicy);

    cameraBtn = new QPushButton("&Camera");
    cameraBtn->setFont(btnFont);
    cameraBtn->setMaximumSize(btnSize);
    cameraBtn->setSizePolicy(btnPolicy);

    // Horizontal layout for buttons
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(databaseBtn);
    btnLayout->addWidget(cameraBtn);
    btnLayout->insertSpacing(1, 40);

    // Create the label at the top
    title = new QLabel("Polymer Hybrid Manufacturing");
    title->setAlignment(Qt::AlignCenter);
    title->setFont(QFont("Futura", 50));

    // Create the banner at the bottom
    banner = new QLabel("LSU College of Engineering");
    banner->setObjectName("banner");
    banner->setFont(QFont("Arial", 30, QFont::Bold));
    banner->setAlignment(Qt::AlignCenter);
    banner->setMaximumHeight(200);
    banner->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    banner->setScaledContents(true);

    // Create a grid layout and add the widgets and layout to it
    QGridLayout* grid = new QGridLayout(this);
    grid->addWidget(title, 0, 0, 1, 2);
    grid->addLayout(btnLayout, 1, 0, 1, 2);
    grid->addWidget(banner, 2, 0, 1, 2);
    grid->setVerticalSpacing(100);
    grid->setHorizontalSpacing(20);

    setLayout(grid);
}
