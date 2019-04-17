#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/core/types_c.h>   

int main(int argc, char** argv) {
    // Verify usage
    if (argc != 2)
        return -1;    
    
    // Open the capture
    cv::Size frameSize(640, 480);
    cv::VideoCapture cap(0);
    cap.set(CV_CAP_PROP_FPS, 15);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, frameSize.height);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, frameSize.width);
    if(!cap.isOpened())
        return -1;

    // Prepare the video destination
    std::string pathname("~/Videos/");
    pathname.append(argv[1]);
    cv::VideoWriter writer(pathname, CV_FOURCC('D','I','V','X'), 15, frameSize);

    // Continually show and record video from the camera with temperature overlay until 'ESC' key is pressed
    for(;;)
    {
        cv::Mat frame;
        cap >> frame;
        putText(frame, "Temperature", cv::Point(0, 470), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 0, 0), 4);
        imshow("Camera", frame);
        writer.write(frame);
        char c = cv::waitKey(25);
        if (c == 27)
            break;
    }
    cv::destroyAllWindows();
    return 0;
}
