//
// Created by murt on 10/8/21.
//

#include "amcrest.h"

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgcodecs.hpp"
#include <experimental/filesystem>
#include "absl/strings/str_join.h"
#include "absl/strings/substitute.h"
#include "absl/flags/flag.h"
#include "absl/time/time.h"
#include "absl/flags/parse.h"
#include <glog/logging.h>
#include "witness/drivers/amcrest.pb.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iostream>
#include <chrono>
#include <thread>

ABSL_FLAG(std::string, connection_file, "", "Amcrest connection textproto");
ABSL_FLAG(std::string, output_dir, "/home/murt/Pictures/witness", "Output directory");

std::string datetime()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,80,"%Y-%m-%d_%H-%M-%S",timeinfo);
    return std::string(buffer);
}

std::experimental::filesystem::path create_folder() {
    std::experimental::filesystem::path new_folder(std::experimental::filesystem::path{absl::GetFlag(FLAGS_output_dir)} / std::experimental::filesystem::path{datetime()});
    std::cout << "Creating folder " << new_folder << std::endl;
    std::experimental::filesystem::create_directories(new_folder);
    return new_folder;
}

void capture_cameras(const AmcrestCameras &file_pb, const std::experimental::filesystem::path &folder, int frame_number) {
    cv::Mat frame;
    for (auto connection: file_pb.connections()) {
        auto amcrest = amcrest::Amcrest(connection);
        const std::string videoStreamAddress = amcrest.get_real_time_stream_url();
//        std::cout << videoStreamAddress << std::endl;

        // Requires FFMpeg built bazel.
        cv::VideoCapture capture;
        if (!capture.open(videoStreamAddress)) {
            std::cout << "  Error opening video stream or file" << std::endl;
        }
        auto image_name = absl::Substitute("$0/$1_$2.png", folder.string(), connection.camera_name(), std::to_string(frame_number));
        std::cout << "  writing image: " << image_name << std::endl;
        capture >> frame;
        cv::imwrite(image_name, frame);
        capture.release();
    }
}

void timelapse(const AmcrestCameras &file_pb, const std::experimental::filesystem::path &folder, int desired_length_seconds, int external_time_in_hours) {
//    for( int i = 0; i < (30 * desired_length_seconds); ++i) {
    int sleep_time = 5;
    int two_days_in_seconds=172800;
    for( int i = 0; i < two_days_in_seconds/sleep_time; ++i) {
        capture_cameras(file_pb, folder, i);
        std::cout << "Sleeping " << sleep_time << " seconds." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(sleep_time) );
        std::cout << "Capturing frame " << i << std::endl;
    }
}

int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = true;
    absl::ParseCommandLine(argc, argv);


    auto file_pb_optional = amcrest::LoadFromTextProto<AmcrestCameras>(absl::GetFlag(FLAGS_connection_file));
    if(!file_pb_optional.has_value()) {
        LOG(ERROR) << "Error reading from textproto file.";
        return -1;
    }
    auto file_pb = file_pb_optional.value();

    auto folder = create_folder();
    timelapse(file_pb, folder, 30, 5);


}
