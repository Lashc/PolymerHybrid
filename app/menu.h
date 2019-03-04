#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

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
    QPushButton* databaseBtn;
    QPushButton* cameraBtn;

signals:
    // Allows the stacked widget to switch to the desired page
    void buttonClicked(int);

public slots:
    // Emits the buttonClicked() signal for the appropiate page in the stacked widget
    void onDatabaseButtonClicked();
};

#endif // MENU_H
