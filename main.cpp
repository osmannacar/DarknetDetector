#include <QCoreApplication>
#include <iostream>
#include "src/darknet.hpp"

#include "opencv2/highgui/highgui.hpp"
#include <string>
#include <chrono>

#define DETECTION_THRESHOLD         0.24
#define DETECTION_HIER_THRESHOLD    0.5
#define NMS_THRESHOLD               0.4

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    cv::VideoCapture cap;
    cv::Mat cvimage;

    Darknet::Image dnimage;
    Darknet::ConvertCvBgr8 converter;
    Darknet::Detector detector;
    std::vector<Darknet::Detection> detections;

    std::string input_names_file("/home/oscar/Yolo/DarknetDetector/cfg/coco.names");
    std::string input_cfg_file("/home/oscar/Yolo/DarknetDetector/cfg/yolov3.cfg");
    std::string input_weights_file("/home/oscar/Yolo/DarknetDetector/cfg/yolov3.weights");

    if (!cap.open("/home/uyar/videos/londonwalk.mp4")) {
        std::cerr << "Could not open video file" << std::endl;
        return -1;
    }

    int image_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int image_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    if (!detector.setup(input_names_file,
                        input_cfg_file,
                        input_weights_file,
                        NMS_THRESHOLD,
                        DETECTION_THRESHOLD,
                        DETECTION_HIER_THRESHOLD,
                        image_width,
                        image_height)) {
        std::cerr << "Setup failed" << std::endl;
        return -1;
    }

    converter.setup(image_width, image_height, detector.get_width(), detector.get_height());
    auto prevTime = std::chrono::system_clock::now();

    while(1) {

        if (!cap.read(cvimage)) {
            std::cerr << "Video capture read failed/EoF" << std::endl;
            return -1;
        }

        // convert and resize opencv image to darknet image
        if (!converter.convert(cvimage, dnimage)) {
            std::cerr << "Failed to convert opencv image to darknet image" << std::endl;
            return -1;
        }

        // run detector
        if (!detector.detect(dnimage)) {
            std::cerr << "Failed to run detector" << std::endl;
            return -1;
        }
        detector.get_detections(detections);

        // draw bounding boxes
        Darknet::image_overlay(detections, cvimage);

        auto now = std::chrono::system_clock::now();
        std::chrono::duration<double> period = (now - prevTime);
        prevTime = now;
        std::cout << "FPS: " << 1 / period.count() << std::endl;

        cv::imshow("Overlay", cvimage);
        cv::waitKey(1);

    }

    return a.exec();
}
