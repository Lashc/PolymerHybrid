// This program is used to view video from a camera and measure
// temperature from a temperature sensor.  It's specifically
// designed to be used with a Raspberry Pi and a Logitech C920 webcam
// but can be used on other platforms and with other cameras.

// These are only used when measuring temperature on a Raspberry Pi
#ifdef __arm__
#include <bcm2835.h>
#include <ctime>
#include <cstdlib>
#endif

#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/core/types_c.h>   

int main() {
    // Open the video capture
    cv::Size frameSize(640, 480);
    cv::VideoCapture capture(0);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, frameSize.height);
    capture.set(CV_CAP_PROP_FRAME_WIDTH, frameSize.width);
    if(!capture.isOpened())
        return -1;

    // Set up the temperature sensor
    #ifdef __arm__
        char buf[6];
        char reg;
        reg = 7;
        double temp = 0, tempShown = 0, sumTemps = 0;
        int numTemps = 0;
        bcm2835_init();
        bcm2835_i2c_begin();
        bcm2835_i2c_set_baudrate(25000);
        bcm2835_i2c_setSlaveAddress(0x5a);
        time_t baseTime = time(NULL);
    #endif
    
    // Continually show video from the camera with temperature overlay until 'ESC' key is pressed
    for(;;)
    {
        // Read from the temperature sensor and update the temperature every 2 seconds
        #ifdef __arm__
            time_t currentTime = time(NULL);
            if (difftime(currentTime, baseTime) >= 2.0) {
                baseTime = currentTime;
                tempShown = sumTemps / numTemps;
                sumTemps = 0.0;
                numTemps = 0;
            }
            bcm2835_i2c_begin();
            bcm2835_i2c_write (&reg, 1);
            bcm2835_i2c_read_register_rs(&reg, &buf[0], 3);
            temp = (double) (((buf[1]) << 8) + buf[0]);
            temp = (temp * 0.02) - 0.01;
            temp = temp - 273.15;
            sumTemps += temp;
            numTemps++;
        #endif
        
        // Retrieve the next frame
        cv::Mat frame;
        capture >> frame;
        
        // Put the temperature as an overlay on the frame and show the frame
        #ifdef __arm__
            std::string tempText("Temp: ");
            tempText.append(std::to_string(tempShown));
            tempText = tempText.substr(0, tempText.find('.') + 3);
            tempText.append(" deg C");
            putText(frame, tempText, cv::Point(0, 470), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 0, 0), 4);
        #endif
        imshow("Camera", frame);
        char c = cv::waitKey(25);
        if (c == 27)
            break;
    }
    cv::destroyAllWindows();
    return 0;
}
