#include "control.h"
#include "menu.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*Control w;
    w.show();*/

    Menu m;
    m.show();
    m.resize(1280, 800);
    return a.exec();
}
