#ifndef CONTROL_H
#define CONTROL_H

#include <QWidget>

class QStackedWidget;

class Control : public QWidget
{
    Q_OBJECT
public:
    explicit Control(QWidget *parent = nullptr);

private:
    QStackedWidget* pages;

public slots:
    void onMenuButtonReleased(int);
    void returnToMainMenu();
};

#endif // CONTROL_H
