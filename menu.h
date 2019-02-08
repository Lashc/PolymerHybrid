#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class Menu : public QWidget
{
    Q_OBJECT
public:
    explicit Menu(QWidget *parent = nullptr);

private:
    QLabel* title;
    QLabel* banner;
    QPushButton* databaseBtn;
    QPushButton* cameraBtn;

signals:

public slots:
};

#endif // MENU_H
