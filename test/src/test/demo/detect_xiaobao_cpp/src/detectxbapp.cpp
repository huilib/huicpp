

#include "detectxbapp.h"
#include <iostream>

#include <opencv2/videoio/videoio.hpp>

DetectXbApp::DetectXbApp(HN argc, HCSZ argv[])
    : base_class(argc, argv) {

}


DetectXbApp::~DetectXbApp() {

}


HRET DetectXbApp::Init () {

    HNOTOK_RETURN(base_class::Init());

    HRETURN_OK;

}


bool DetectXbApp::Run () {

    cv::VideoCapture video_cam;
    if (not video_cam.open("/home/joseph/Documents/tasks/detect_face/bbb.mp4")) {
        std::cout << "open video cam failed" << std::endl;
        return false;
    }

    if (not video_cam.isOpened()) {
        std::cout << "video cam is not opened" << std::endl;
        return false;
    }

    int ccc = 0;

    while (true) {

        cv::Mat img;

        video_cam >> img;

        if (img.empty()) {
            break;
        }

        ++ccc;
    }

    std::cout << "get " << ccc << " frames" << std::endl;

    return true;

}

//pkg-config opencv4 --libs --cflags --libs-only-L