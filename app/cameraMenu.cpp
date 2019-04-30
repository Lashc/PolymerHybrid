#include "cameraMenu.h"
#include <cstdlib>
#include <string>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QInputDialog>
#include <QRegularExpressionValidator>
#include <QMessageBox>

CameraMenu::CameraMenu(QWidget *parent)
    : QWidget(parent)
{
    // Header for menu
    cameraHeader = new QLabel("Camera Functions");
    cameraHeader->setFont(QFont("Futura", 50));
    cameraHeader->setStyleSheet("QLabel { color: rgb(0, 35, 102); }");
    cameraHeader->setAlignment(Qt::AlignCenter);

    // Push buttons for interacting with a camera and temperature sensor as well as going back
    // to the main menu
    QSize pushBtnMinSize(150, 100);
    QSize pushBtnMaxSize(400, 200);
    QSizePolicy pushBtnSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred, QSizePolicy::PushButton);
    QFont pushBtnFont("Gotham", 30, QFont::Medium);
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
    btnLayout->addWidget(viewBtn, 0, 0);
    btnLayout->addWidget(recordViewBtn, 0, 1);
    btnLayout->addWidget(backBtn, 1, 0, 1, 2);
    btnLayout->setVerticalSpacing(30);
    layout->addSpacing(50);
    layout->setMargin(10);
    layout->setSpacing(25);
    setLayout(layout);

    // Connect signals and slots
    connect(backBtn, SIGNAL(released()), this->parentWidget(), SLOT(returnToMainMenu()));
    connect(viewBtn, SIGNAL(released()), this, SLOT(viewCamera()));
    connect(recordViewBtn, SIGNAL(released()), this, SLOT(viewAndRecordCamera()));
}

void CameraMenu::viewCamera()
{
    // Execute a pre-compiled program for viewing camera feed
    #ifdef __arm__
        system("sudo ~/./viewVideo");
    #else
        system("~/./viewVideo");
    #endif
}

void CameraMenu::viewAndRecordCamera()
{
    // Get a filename from the user
    bool ok;
    QString filename = QInputDialog::getText(this, "Video filename", "Please enter a filename for the video (*.avi):", QLineEdit::Normal, "", &ok);
    QRegularExpressionValidator fileValidator(QRegularExpression("^(\\w)+[.]avi$"), this);
    int pos = 0;
    if (ok && (fileValidator.validate(filename, pos) != QValidator::Acceptable || filename.length() > 255)) {
        // Invalid filename
        QMessageBox invalidFilenameDialog(QMessageBox::Warning, "Invalid filename",
                             "The filename entered is invalid.  The filename must be in the "
                             "form *.avi (e.g. video.avi) and no more than 255 characters long.", QMessageBox::Ok, this);
        invalidFilenameDialog.exec();
        return;
    }
    // Execute a pre-compiled program for viewing and recording camera feed
    std::string command;
    #ifdef __arm__
        command = "sudo ~/./viewRecordVideo ";
    #else
        command = "~/./viewRecordVideo ";
    #endif
    command.append(filename.toStdString());
    system(command.c_str());
}
