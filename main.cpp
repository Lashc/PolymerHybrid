#include "control.h"
#include "menu.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Control gui;
    gui.show();
    gui.setWindowTitle("Polymer Hybrid Manufacturing Data System");
    gui.resize(1280, 800);
    return a.exec();
}
