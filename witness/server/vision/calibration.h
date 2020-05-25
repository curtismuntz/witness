#pragma once

#include <vector>
#include <string>

#include "glog/logging.h"

#include "opencv2/calib3d.hpp"
#include "opencv2/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"

namespace witness::server::vision {

static const unsigned int kMinimumBoardSizeWidth = 1;
static const unsigned int kMinimumBoardSizeHeight = 1;
static const unsigned int kMinimumWindowSize = 1;
static const unsigned int kMinimumCalibrationSamples = 8;

using namespace cv;

struct CalibrateParameters {
  unsigned int board_size_width;
  unsigned int board_size_height;
  unsigned int square_size_mm;
  unsigned int window_size;
  unsigned int minimum_calibration_samples;
  bool use_fisheye;
  int flag;
  float aspectRatio;
  bool fixK1;
  bool fixK2;
  bool fixK3;
  bool fixK4;
  bool fixK5;
  bool calibFixPrincipalPoint;
  bool calibZeroTangentDist;
  cv::Size boardSize;

  CalibrateParameters()
      : board_size_width(0),
        board_size_height(0),
        square_size_mm(0),
        window_size(0),
        minimum_calibration_samples(0),
        use_fisheye(false),
        flag(0),
        fixK1(false),
        fixK2(false),
        fixK3(false),
        fixK4(false),
        fixK5(false),
        aspectRatio(false),
        calibFixPrincipalPoint(false),
        calibZeroTangentDist(false),
        boardSize(0, 0) {
    calculate_flag();
  }

  void calculate_flag() {
    flag = CALIB_FIX_K4 | CALIB_FIX_K5;
    if (calibFixPrincipalPoint) flag |= CALIB_FIX_PRINCIPAL_POINT;
    if (calibZeroTangentDist) flag |= CALIB_ZERO_TANGENT_DIST;
    if (aspectRatio) flag |= CALIB_FIX_ASPECT_RATIO;

    if (use_fisheye) {
      // the fisheye model has its own enum, so overwrite the flags
      flag = fisheye::CALIB_FIX_SKEW | fisheye::CALIB_RECOMPUTE_EXTRINSIC |
             // fisheye::CALIB_FIX_K1 |
             fisheye::CALIB_FIX_K2 | fisheye::CALIB_FIX_K3 | fisheye::CALIB_FIX_K4;
    }
  }

  bool validate() {
    bool valid = true;
    if (board_size_width < kMinimumBoardSizeWidth) {
      valid = false;
    }
    if (board_size_height < kMinimumBoardSizeHeight) {
      valid = false;
    }
    boardSize.width = board_size_width;
    boardSize.height = board_size_height;
    if (window_size < kMinimumWindowSize) {
      valid = false;
    }
    if (minimum_calibration_samples < kMinimumCalibrationSamples) {
      LOG(ERROR) << "Requested minimum_calibration_samples" << minimum_calibration_samples
                 << "was less than the required " << kMinimumCalibrationSamples << " samples";
      valid = false;
    }
    return valid;
  }
};

class Calibrator {
 public:
  explicit Calibrator(const CalibrateParameters &params);
  bool add_image(const cv::Mat &img);
  bool add_image_from_file(const std::string &fname);
  bool calibrate();
  cv::Mat get_camera_matrix() { return camera_matrix_; }
  bool find_chess_board(const cv::Mat &m, std::vector<Point2f> &pointBuf);

 private:
  bool runCalibration(CalibrateParameters &s, Size &imageSize, Mat &cameraMatrix, Mat &distCoeffs,
                      std::vector<std::vector<Point2f>> imagePoints, std::vector<Mat> &rvecs,
                      std::vector<Mat> &tvecs, std::vector<float> &reprojErrs, double &totalAvgErr);

  bool runCalibrationAndSave(CalibrateParameters &s, Size imageSize, Mat &cameraMatrix,
                             Mat &distCoeffs, std::vector<std::vector<Point2f>> imagePoints);

  void calcBoardCornerPositions(Size boardSize, float squareSize, std::vector<Point3f> &corners);
  double computeReprojectionErrors(const std::vector<std::vector<Point3f>> &objectPoints,
                                   const std::vector<std::vector<Point2f>> &imagePoints,
                                   const std::vector<Mat> &rvecs, const std::vector<Mat> &tvecs,
                                   const Mat &cameraMatrix, const Mat &distCoeffs,
                                   std::vector<float> &perViewErrors, bool fisheye);
  CalibrateParameters params_;
  std::vector<cv::Mat> image_list_;
  cv::Mat camera_matrix_;
};

}  // namespace witness::server::vision
