#include <opencv2/opencv.hpp>
#include <opencv2/core/types_c.h>
    
int main() {
    // Open the capture
    cv::Size frameSize(640, 480);
    cv::VideoCapture cap(0);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, frameSize.height);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, frameSize.width);
    if(!cap.isOpened())
        return -1;

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
    return 0;
}
