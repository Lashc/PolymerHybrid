#ifndef CAMERAMENU_H
#define CAMERAMENU_H

#include <QWidget>

class QPushButton;
class QLabel;

// Menu for using a camera to view and record prints and measure temperature
class CameraMenu : public QWidget
{
    Q_OBJECT
public:
    explicit CameraMenu(QWidget *parent = nullptr);

private:
    QLabel* cameraHeader;
    QPushButton* viewBtn;
    QPushButton* recordViewBtn;
    QPushButton* backBtn;

signals:

public slots:
    void viewCamera();
    void viewAndRecordCamera();
};

#endif // CAMERAMENU_H
