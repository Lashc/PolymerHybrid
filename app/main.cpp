#include "control.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set the stylesheet
    QFile qssFile(":/menu_style.qss");
    qssFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(qssFile.readAll());
    a.setStyleSheet(styleSheet);
    qssFile.close();

    // The main widget
    Control gui;
    gui.show();
    gui.setWindowTitle("Polymer Hybrid Manufacturing Data System");
    #ifdef __arm__
        gui.showMaximized();
    #else
        gui.resize(1280, 800);
    #endif

    return a.exec();
}
