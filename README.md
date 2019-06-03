# Polymer Hybrid Manufacturing GUI

This project was for my two-semester, interdisciplinary senior design class at Louisiana State University (EE 4810/EE 4820).  The objective of the project as a whole was to integrate a CNC (Computer Numerical Control) mini mill with a prototype polymer extrusion head in order to perform hybrid manufacturing using polymers.  My role for the project was to create a GUI that allows operators of the system to easily store process parameters, test results, and defect information in a local database, monitor the process via a camera and a temperature sensor, and browse a custom operations manual.  The GUI was acessible via a 1280 x 800 touchscreen monitor mounted on the mini mill.  A Logitech C920 webcam was used for the camera.  A Raspberry Pi 3 Model B+ hosted the GUI and the database.  The GUI was designed with these target devices in mind.

## Main Menu

The main menu allows users to access the individual sections of the GUI, those being the database menu, camera menu, and operations manual.

![Main Menu](https://user-images.githubusercontent.com/35757768/58774729-854d6880-8588-11e9-80b5-f73a9b2d91e6.png)

## Database Menu

The database menu allows users to store, retrieve, modify, and delete database entries.  Users must select a specific category to perform operations on those sets of data. 

![Database Menu](https://user-images.githubusercontent.com/35757768/58775178-ecb7e800-8589-11e9-9fe1-c502d9fa3ed3.png)

By clicking on a row number in the left column, a dialog will open that shows the data for a row in a concise format.  Different forms/dialogs will open when users click any of the "Add Entry", "Modify Entry", and "Delete Entry" buttons.  The following image shows a sample form for adding a print parameters entry.  The fields with a red border around them are required and must be non-empty upon submission.

![Empty Print Parameters Form](https://user-images.githubusercontent.com/35757768/58775505-6a302800-858b-11e9-92a2-7974351cba3a.png)

In addition to verifying the existence of required fields, other input validation mechanisms were implemented.  For instance, dates must be in the specified format, and humidity values must lie between 0% and 100%.  If any values are invalid upon submission, a dialog will appear that informs users of which fields are invalid.  Users may then modify invalid values and attempt to resubmit.

## Camera Menu

The camera menu allows users to view and record the hybrid manufacturing process via a webcam.  Recorded videos are stored in the current user's "Videos" folder in their home directory.

![Camera Menu](https://user-images.githubusercontent.com/35757768/58775833-23dbc880-858d-11e9-9647-93eea01762d2.png)

The temperature of the material as it's being extruded is also measured via an [MLX90614 infrared temperature sensor](https://acrobotic.com/products/brk-00018) and displayed on-screen.  The following image was captured during a purge of the extruder head (removal of excess material after printing).

![Webcam Screenshot](https://user-images.githubusercontent.com/35757768/58775805-01e24600-858d-11e9-8a03-515a8789fea5.png)

## Operations Manual

The custom operations manual guides users on how to operate the system in a step-by-step fashion.  The operations manual can be read in a PDF viewer by clicking the "Operations Manual" button on the main menu.  The manual must reside in the "Documents" folder of the the user's home directory.  The following screenshot shows a sample of the manual.

![Operations Manual](https://user-images.githubusercontent.com/35757768/58775913-8df46d80-858d-11e9-88c5-c90cb8c5ca4f.png)

## Installation

### Dependencies

  * [Qt](https://www.qt.io) (Any version of Qt 5 should suffice; Qt 5.10.1 was used) - Framework for creating the GUI
  * [OpenCV](https://opencv.org) (Version 2.4.1 or greater) - Library for processing camera footage
  * [bcm2835](https://www.airspayce.com/mikem/bcm2835/) - C library for temperature sensor (only needed when using Raspberry Pi)
  
If using Linux, OpenCV (although an outdated version) can be installed via the terminal using `sudo apt-get install libopencv-dev`.  To run this project on a Raspberry Pi, Qt must be cross-compiled for the Raspberry Pi.  Please refer to this [guide](https://mechatronicsblog.com/cross-compile-and-deploy-qt-5-12-for-raspberry-pi/) for cross-compiling Qt.  I was unable to run the GUI as an X11 window using Qt 5.12, but I was was able to accomplish this using Qt 5.10.1.

### Building the project

The easiest way to build this project is to use Qt Creator, which is an IDE made by the Qt Company.  Create a new project in Qt Creator by importing this repository.  Press the green play button to build the project and run it.

In order to access a camera feed, the files "viewVideo.cpp" and "viewRecordVideo.cpp" must be compiled separately from the rest of the files.  After installing OpenCV, these files can be compiled in Linux by navigating to the project directory in the terminal and inputting `` g++ viewVideo.cpp -o ~/viewVideo `pkg-config --cflags --libs opencv` `` and `` g++ viewRecordVideo.cpp -o ~/viewRecordVideo `pkg-config --cflags --libs opencv` ``.  If compiling for a Raspberry Pi, the bcm2835 library must be linked by suffixing these commands with `-l bcm2835`.

## License

This project is under the MIT License.  Please see [LICENSE](./LICENSE) for details.

## Acknowledgments

Special thanks to Stack Overflow user Eddie Fiorentine's [starter code](https://stackoverflow.com/questions/35880952/raspberry-pi-interfacing-with-an-i2c-enabled-ir-temp-sensor-mlx90614) for the temperature sensor.  Also thanks to the Qt Company for their amazing framework!
