#include "menu.h"
#include <QGridLayout>

Menu::Menu(QWidget *parent) : QWidget(parent)
{
    QGridLayout* grid = new QGridLayout(this);

    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    databaseBtn = new QPushButton("&Database");
    cameraBtn = new QPushButton("&Camera");
    databaseBtn->setSizePolicy(sizePolicy);
    cameraBtn->setSizePolicy(sizePolicy);

    title = new QLabel("Polymer Hybrid Manufacturing");
    title->setAlignment(Qt::AlignHCenter);
    title->setFont(QFont("Futura", 19));;
    banner = new QLabel("LSU College of Engineering");
    banner->setAlignment(Qt::AlignCenter);
    banner->setStyleSheet("color: rgb(53, 13, 104); background-color: rgb(253, 208, 35); border-color: rgb(53, 13, 104); border-width: 5px; border-style: solid");

    grid->addWidget(title, 0, 0, 1, 2);
    grid->addWidget(databaseBtn, 1, 0);
    grid->addWidget(cameraBtn, 1, 1);
    grid->addWidget(banner, 2, 0, 1, 2);
    grid->setVerticalSpacing(50);
    grid->setHorizontalSpacing(20);

    setLayout(grid);
}
