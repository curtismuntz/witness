#include "witness/server/webcam/actions/tracking.h"
#include "witness/server/common/file_operations.h"

extern "C" {
#include "apriltag/apriltag.h"
#include "apriltag/tagStandard41h12.h"
}

namespace witness {
namespace server {
namespace webcam {
namespace actions {

Tracking::Tracking(std::shared_ptr<witness::webcam::Webcam> webcam, const std::string &fname, const std::string &tag_id)
  : WebcamAction(webcam)
  , fname_(fname)
  , tag_id_(tag_id)
{};


bool Tracking::Loop() {
  webcam_->OpenCamera();
  auto desired_filename = "tracking.avi";
  cv::Mat gray;
  cv::Mat frame;
  int frame_number = 0;


  auto video = webcam_->CreateVideoObject(desired_filename);
  apriltag_family_t *tf = NULL;
  tf = tagStandard41h12_create();
  apriltag_detector_t *td = apriltag_detector_create();
  apriltag_detector_add_family(td, tf);

  // TODO(curtismuntz): configurables!
  td->quad_decimate = 2.0;
  td->quad_sigma = 0.0;
  td->nthreads = 1;
  td->debug = false;
  td->refine_edges = true;

  while (working_ && webcam_->camera_->isOpened()) {
    webcam_->ReadGrey(&frame, &gray);
    // Make an image_u8_t header for the Mat data
    image_u8_t im = { .width = gray.cols,
        .height = gray.rows,
        .stride = gray.cols,
        .buf = gray.data
    };

    zarray_t *detections = apriltag_detector_detect(td, &im);


    LOG(INFO) << zarray_size(detections) << "tags detected";

    // Draw detection outlines
    for (int i = 0; i < zarray_size(detections); i++) {
        apriltag_detection_t *det;
        zarray_get(detections, i, &det);
        cv::line(frame, cv::Point(det->p[0][0], det->p[0][1]),
                 cv::Point(det->p[1][0], det->p[1][1]),
                 cv::Scalar(0, 0xff, 0), 2);
        cv::line(frame, cv::Point(det->p[0][0], det->p[0][1]),
                 cv::Point(det->p[3][0], det->p[3][1]),
                 cv::Scalar(0, 0, 0xff), 2);
        cv::line(frame, cv::Point(det->p[1][0], det->p[1][1]),
                 cv::Point(det->p[2][0], det->p[2][1]),
                 cv::Scalar(0xff, 0, 0), 2);
        cv::line(frame, cv::Point(det->p[2][0], det->p[2][1]),
                 cv::Point(det->p[3][0], det->p[3][1]),
                 cv::Scalar(0xff, 0, 0), 2);

        std::stringstream ss;
        ss << det->id;
        cv::String text = ss.str();
        int fontface = CV_FONT_HERSHEY_SCRIPT_SIMPLEX;
        double fontscale = 1.0;
        int baseline;
        cv::Size textsize = cv::getTextSize(text, fontface, fontscale, 2,
                                        &baseline);
        cv::putText(frame, text, cv::Point(det->c[0]-textsize.width/2,
                                   det->c[1]+textsize.height/2),
                fontface, fontscale, cv::Scalar(0xff, 0x99, 0), 2);
    }
    apriltag_detections_destroy(detections);
    video.write(frame);
    ++frame_number;
  }
  apriltag_detector_destroy(td);
  tagStandard41h12_destroy(tf);
  video.release();


  webcam_->CloseCamera();
}

}  // namespace actions
}  // namespace webcam
}  // namespace server
}  // namespace witness
