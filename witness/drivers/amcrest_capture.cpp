#include <chrono>
#include <ctime>
#include <experimental/filesystem>

#include <glog/logging.h>
#include <iomanip>
//#include <iostream>
#include <thread>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
//#include "absl/strings/str_join.h"
#include "absl/strings/substitute.h"
#include "indicators/progress_bar.hpp"
#include "indicators/cursor_control.hpp"


#include "witness/drivers/amcrest.pb.h"
#include "witness/server/vision/create_timelapse.h"
#include "witness/server/file_operations/file_operations.h"


#include "amcrest.h"

//#include <thread>
ABSL_FLAG(std::string, connection_file, "", "Amcrest connection textproto");
ABSL_FLAG(std::string, output_dir, "/home/murt/Pictures/witness", "Output directory");
ABSL_FLAG(int, external_hours, 1, "Real life hours to run timelapse for");


void capture_cameras(const AmcrestCameras &file_pb, const std::experimental::filesystem::path &folder, int frame_number) {
    for (auto connection: file_pb.connections()) {
        auto amcrest = amcrest::Amcrest(connection);
        const std::string videoStreamAddress = amcrest.get_real_time_stream_url();
        std::string photoEndpoint = amcrest.get_photo_url();

        std::stringstream frame_number_filled;
        frame_number_filled << std::setw(4) << std::setfill('0') << frame_number;

        auto image_name = absl::Substitute("$0/$1_$2.jpg", folder.string(), connection.camera_name(), frame_number_filled.str());
        DLOG(INFO) << "  writing image: " << image_name << std::endl;
        amcrest.download_jpeg(photoEndpoint.c_str(), image_name);
    }
}

void timelapse(const AmcrestCameras &file_pb, const std::experimental::filesystem::path &folder, int desired_length_seconds, int external_time_in_hours) {
    const static int kFrameRate = 30;
    static int timelapse_frames = desired_length_seconds * kFrameRate;
    const int sleep_time = external_time_in_hours * 4;
    LOG(INFO) << "TOTAL TIMELAPSE DURATION: " << (timelapse_frames * sleep_time / 60) << " minutes.";
    LOG(INFO) << "Sleeping " << sleep_time << "s between captures.";

    using namespace indicators;
    show_console_cursor(false);
    ProgressBar bar{
        option::BarWidth{150},
        option::Start{" ["},
        option::Fill{"█"},
        option::Lead{"█"},
        option::Remainder{"-"},
        option::End{"]"},
        option::PrefixText{"Capturing frames 📷"},
        option::ForegroundColor{Color::yellow},
        option::ShowElapsedTime{true},
        option::ShowRemainingTime{true},
        option::MaxProgress{timelapse_frames},
        option::FontStyles{std::vector<FontStyle>{FontStyle::bold}}
    };
    for( int i = 0; i < timelapse_frames; ++i) {
        // Show iteration as postfix text
        bar.set_option(option::PostfixText{
          std::to_string(i) + "/" + std::to_string(timelapse_frames)
        });
        bar.tick();
        DLOG(INFO) << "Capturing frame " << i << std::endl;
        capture_cameras(file_pb, folder, i);
        DLOG(INFO) << "Sleeping " << sleep_time << " seconds.";
        std::this_thread::sleep_for(std::chrono::seconds(sleep_time) );
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

    auto folder = witness::server::file_operations::MakeTimeDir(absl::GetFlag(FLAGS_output_dir));
    timelapse(file_pb, folder, 30, absl::GetFlag(FLAGS_external_hours));
    witness::server::vision::create_timelapse(folder, {".jpg"});
}
