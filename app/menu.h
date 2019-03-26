#ifndef MENU_H
#define MENU_H

#include <QWidget>

class QLabel;
class QButtonGroup;
class QPushButton;

// Class for the main menu, which allows you to select between two sub-menus: "Database" and "Camera"
class Menu : public QWidget
{
    Q_OBJECT
public:
    explicit Menu(QWidget *parent = nullptr);

private:
    // Widgets on main menu
    QLabel* title;
    QLabel* banner;
    QButtonGroup* btnGroup;
    QPushButton* databaseBtn;
    QPushButton* cameraBtn;
    QPushButton* manualBtn;

signals:
    // Allows the stacked widget to switch to the desired page
    void buttonReleased(int);

public slots:
    // Emits the buttonReleased() signal for the appropiate page in the stacked widget
    void onButtonReleased(int);
};

#endif // MENU_H
