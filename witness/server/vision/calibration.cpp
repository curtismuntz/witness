#include "witness/server/vision/calibration.h"
#include "witness/server/file_operations/file_operations.h"

namespace witness {
namespace server {
namespace vision {

Calibrator::Calibrator(const CalibrateParameters &params)
    : params_(params), camera_matrix_(cv::Mat::eye(3, 3, 1)) {}

bool Calibrator::add_image_from_file(const std::string &fname) {
  if (!witness::server::file_operations::exists(fname)) {
    LOG(WARNING) << "File " << fname << " does not exist";
    return false;
  }
  return add_image(cv::imread(fname));
}

// Assumes a valid chessboard is seen
bool Calibrator::add_image(const cv::Mat &img) {
  if (img.empty()) {
    LOG(WARNING) << "Skipping empty image";
    return false;
  }
  image_list_.push_back(img);
  return true;
}

bool Calibrator::find_chess_board(const cv::Mat &m, std::vector<Point2f> &pointBuf) {
  auto imageSize = m.size();  // Format input image.
  bool found;

  int chessBoardFlags = CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE;
  if (!params_.use_fisheye) {
    // fast check erroneously fails with high distortions like fisheye
    chessBoardFlags |= CALIB_CB_FAST_CHECK;
  }
  found = findChessboardCorners(m, params_.boardSize, pointBuf, chessBoardFlags);
  return found;
}

// Logic taken from
// https://raw.githubusercontent.com/opencv/opencv/3.1.0/samples/cpp/tutorial_code/calib3d/camera_calibration/camera_calibration.cpp
bool Calibrator::calibrate() {
  if (!params_.validate()) {
    LOG(ERROR) << "Cannot calibrate due to invalid parameters";
    return false;
  }

  LOG(INFO) << "Calibration starting";

  std::vector<std::vector<Point2f>> imagePoints;
  Mat cameraMatrix, distCoeffs;
  Size imageSize;
  const Scalar RED(0, 0, 255), GREEN(0, 255, 0);
  const char ESC_KEY = 27;
  for (auto view : image_list_) {
    imageSize = view.size();  // Format input image.
    std::vector<Point2f> pointBuf;

    bool found = find_chess_board(view, pointBuf);
    if (found) {
      DLOG(INFO) << " chessboard found";
      Mat viewGray;
      cvtColor(view, viewGray, COLOR_BGR2GRAY);
      cornerSubPix(viewGray, pointBuf, Size(11, 11), Size(-1, -1),
                   TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 30, 0.1));
      imagePoints.push_back(pointBuf);
    }
  }
  if (imagePoints.size() < 1) {
    LOG(INFO) << "unable to run calibration on this data, insufficient image points: "
              << imagePoints.size();
    return false;
  }
  LOG(INFO) << "Running calibration";
  return runCalibrationAndSave(params_, imageSize, cameraMatrix, distCoeffs, imagePoints);
}

bool Calibrator::runCalibrationAndSave(CalibrateParameters &s, Size imageSize, Mat &cameraMatrix,
                                       Mat &distCoeffs,
                                       std::vector<std::vector<Point2f>> imagePoints) {
  std::vector<Mat> rvecs, tvecs;
  std::vector<float> reprojErrs;
  double totalAvgErr = 0;

  bool ok = runCalibration(s, imageSize, cameraMatrix, distCoeffs, imagePoints, rvecs, tvecs,
                           reprojErrs, totalAvgErr);
  LOG(INFO) << (ok ? "Calibration succeeded" : "Calibration failed")
            << ". avg re projection error = " << totalAvgErr;

  // if (ok)
  //     saveCameraParams(s, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs, reprojErrs,
  //     imagePoints,
  //                      totalAvgErr);

  if (ok) {
    LOG(INFO) << "CameraMatrix: ";
    LOG(INFO) << cameraMatrix;
    camera_matrix_ = cameraMatrix;
  }
  return ok;
}

bool Calibrator::runCalibration(CalibrateParameters &s, Size &imageSize, Mat &cameraMatrix,
                                Mat &distCoeffs, std::vector<std::vector<Point2f>> imagePoints,
                                std::vector<Mat> &rvecs, std::vector<Mat> &tvecs,
                                std::vector<float> &reprojErrs, double &totalAvgErr) {
  //! [fixed_aspect]
  cameraMatrix = Mat::eye(3, 3, CV_64F);
  if (s.flag & CALIB_FIX_ASPECT_RATIO) cameraMatrix.at<double>(0, 0) = s.aspectRatio;
  //! [fixed_aspect]
  if (s.use_fisheye) {
    distCoeffs = Mat::zeros(4, 1, CV_64F);
  } else {
    distCoeffs = Mat::zeros(8, 1, CV_64F);
  }

  std::vector<std::vector<Point3f>> objectPoints(1);
  calcBoardCornerPositions(s.boardSize, s.square_size_mm, objectPoints[0]);

  objectPoints.resize(imagePoints.size(), objectPoints[0]);

  // Find intrinsic and extrinsic camera parameters
  double rms;

  if (s.use_fisheye) {
    Mat _rvecs, _tvecs;
    rms = fisheye::calibrate(objectPoints, imagePoints, imageSize, cameraMatrix, distCoeffs, _rvecs,
                             _tvecs, s.flag);

    rvecs.reserve(_rvecs.rows);
    tvecs.reserve(_tvecs.rows);
    for (int i = 0; i < static_cast<int>(objectPoints.size()); i++) {
      rvecs.push_back(_rvecs.row(i));
      tvecs.push_back(_tvecs.row(i));
    }
  } else {
    rms = calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix, distCoeffs, rvecs,
                          tvecs, s.flag);
  }

  LOG(INFO) << "Re-projection error reported by calibrateCamera: " << rms;

  bool ok = checkRange(cameraMatrix) && checkRange(distCoeffs);

  totalAvgErr = computeReprojectionErrors(objectPoints, imagePoints, rvecs, tvecs, cameraMatrix,
                                          distCoeffs, reprojErrs, s.use_fisheye);

  return ok;
}

// TODO(curtismuntz) support more than just chessboard
void Calibrator::calcBoardCornerPositions(Size boardSize, float squareSize,
                                          std::vector<Point3f> &corners) {
  corners.clear();
  for (int i = 0; i < boardSize.height; ++i) {
    for (int j = 0; j < boardSize.width; ++j) {
      corners.push_back(Point3f(j * squareSize, i * squareSize, 0));
    }
  }
}

double Calibrator::computeReprojectionErrors(const std::vector<std::vector<Point3f>> &objectPoints,
                                             const std::vector<std::vector<Point2f>> &imagePoints,
                                             const std::vector<Mat> &rvecs,
                                             const std::vector<Mat> &tvecs, const Mat &cameraMatrix,
                                             const Mat &distCoeffs,
                                             std::vector<float> &perViewErrors, bool fisheye) {
  std::vector<Point2f> imagePoints2;
  size_t totalPoints = 0;
  double totalErr = 0, err;
  perViewErrors.resize(objectPoints.size());

  for (size_t i = 0; i < objectPoints.size(); ++i) {
    if (fisheye) {
      fisheye::projectPoints(objectPoints[i], imagePoints2, rvecs[i], tvecs[i], cameraMatrix,
                             distCoeffs);
    } else {
      projectPoints(objectPoints[i], rvecs[i], tvecs[i], cameraMatrix, distCoeffs, imagePoints2);
    }
    err = norm(imagePoints[i], imagePoints2, NORM_L2);

    size_t n = objectPoints[i].size();
    perViewErrors[i] = static_cast<float>(std::sqrt(err * err / n));
    totalErr += err * err;
    totalPoints += n;
  }

  return std::sqrt(totalErr / totalPoints);
}

}  // namespace vision
}  // namespace server
}  // namespace witness
