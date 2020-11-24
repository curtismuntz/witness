#include <stdio.h>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgcodecs.hpp"
#include <iostream>
#include "gflags/gflags.h"

DEFINE_string(username, "", "Username for RTSP camera");
DEFINE_string(password, "", "Password for RTSP camera");
DEFINE_string(hostname, "", "Hostname of RTSP camera");
DEFINE_string(port, "", "Port of RTSP camera");

class Amcrest {
public:
    Amcrest(const std::string &username, const std::string &password, const std::string &hostname,
            const std::string &port)
            : username_(username),
              password_(password),
              hostname_(hostname),
              port_(port) {

    }

    std::string const auth() {
        std::stringstream ss;
        ss << username_ << ":" << password_;
        return ss.str();
    }
    std::string const host_string() {
        std::stringstream ss;
        ss << hostname_ << ":" << port_;
        return ss.str();
    }
    std::string const get_authd_host_string() {
        std::stringstream ss;
        ss << auth() << "@" << host_string();
        return ss.str();
    }
    std::string const get_real_time_stream_url(const std::string &channel, const std::string &subtype) {
        std::stringstream ss;
        ss << "rtsp://" << get_authd_host_string() << "/cam/realmonitor?channel=" << channel << "&subtype=" << subtype;
        return ss.str();
    }
private:
    const std::string username_, password_, hostname_, port_;
};


int main(int argc, char** argv) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    cv::VideoCapture vcap;
    cv::Mat frame;

    auto amcrest = Amcrest(FLAGS_username, FLAGS_password, FLAGS_hostname, FLAGS_port);


    const std::string videoStreamAddress = amcrest.get_real_time_stream_url("1", "1");
    std::cout << videoStreamAddress << std::endl;

    // Requires FFMpeg built bazel
    cv::VideoCapture capture;
    if(!capture.open(videoStreamAddress)) {
        // if(!capture.isOpened()) {
        std::cout << "Error opening video stream or file" << std::endl;
        return -1;
    }

    int inner_iter = 0;

    int frame_width = capture.get(cv::CAP_PROP_FRAME_WIDTH);

    int frame_height = capture.get(cv::CAP_PROP_FRAME_HEIGHT);
//    auto codec = capture.get(cv::CAP_PROP_FOURCC);
    auto codec = cv::VideoWriter::fourcc('X', '2', '6', '4');
    cv::VideoWriter video("out.avi", codec, 20, cv::Size(frame_width, frame_height));

//    video.open("out.mp4",CV_FOURCC('M','J','P','G'),10, cv::Size(frame_width,frame_height));

    while (capture.isOpened()) {

        // get a new frame from camera
        capture >> frame;
        video.write(frame);
    }
    video.release();
}
