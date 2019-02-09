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
    m.resize(800, 600);
    return a.exec();
}
