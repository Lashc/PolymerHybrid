#include "cameraMenu.h"
#include <cstdlib>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <opencv2/opencv.hpp>
#include <opencv2/core/types_c.h>

CameraMenu::CameraMenu(QWidget *parent)
    : QWidget(parent)
{
    // Header for menu
    cameraHeader = new QLabel("Camera Functions");
    cameraHeader->setFont(QFont("Gotham", 50));
    cameraHeader->setStyleSheet("QLabel { color: rgb(0, 35, 102); }");
    cameraHeader->setAlignment(Qt::AlignCenter);

    // Push buttons for interacting with a camera and temperature sensor as well as going back
    // to the main menu
    QSize pushBtnMinSize(150, 100);
    QSize pushBtnMaxSize(300, 200);
    QSizePolicy pushBtnSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred, QSizePolicy::PushButton);
    QFont pushBtnFont("Gotham", 20, QFont::Medium);
    recordBtn = new QPushButton("Record video");
    recordBtn->setMinimumSize(pushBtnMinSize);
    recordBtn->setMaximumSize(pushBtnMaxSize);
    recordBtn->setSizePolicy(pushBtnSizePolicy);
    recordBtn->setFont(pushBtnFont);
    viewBtn = new QPushButton("View camera");
    viewBtn->setMinimumSize(pushBtnMinSize);
    viewBtn->setMaximumSize(pushBtnMaxSize);
    viewBtn->setSizePolicy(pushBtnSizePolicy);
    viewBtn->setFont(pushBtnFont);
    recordViewBtn = new QPushButton("Record and\nview camera");
    recordViewBtn->setMinimumSize(pushBtnMinSize);
    recordViewBtn->setMaximumSize(pushBtnMaxSize);
    recordViewBtn->setSizePolicy(pushBtnSizePolicy);
    recordViewBtn->setFont(pushBtnFont);
    backBtn = new QPushButton("Back");
    backBtn->setMinimumSize(pushBtnMinSize);
    backBtn->setMaximumHeight(pushBtnMaxSize.height());
    backBtn->setSizePolicy(pushBtnSizePolicy);
    backBtn->setFont(pushBtnFont);

    // Configure main layout and layout for push buttons
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(cameraHeader, 1);
    QGridLayout* btnLayout = new QGridLayout;
    layout->addLayout(btnLayout, 3);
    btnLayout->addWidget(recordBtn, 0, 0);
    btnLayout->addWidget(viewBtn, 0, 1);
    btnLayout->addWidget(recordViewBtn, 0, 2);
    btnLayout->addWidget(backBtn, 1, 0, 1, 3);
    btnLayout->setSpacing(30);
    layout->addSpacing(50);
    layout->setMargin(10);
    layout->setSpacing(25);
    setLayout(layout);

    // Connect signals and slots
    connect(backBtn, SIGNAL(released()), this->parentWidget(), SLOT(returnToMainMenu()));
    connect(viewBtn, SIGNAL(released()), this, SLOT(viewCamera()));
}

void CameraMenu::viewCamera()
{
    // Open the capture
    cv::VideoCapture cap(0);
    if(!cap.isOpened())
        return;

    // Continually show video from the camera with temperature overlay until 'ESC' key is pressed
    for(;;)
    {
        cv::Mat frame;
        cap >> frame;
        putText(frame, "Temperature", cv::Point(0, 470), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 0, 0), 4);
        imshow("Camera", frame);
        char c = cv::waitKey(25);
        if (c == 27)
            break;
    }
    cv::destroyAllWindows();
    return;
}
