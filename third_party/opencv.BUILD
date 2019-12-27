# BUILD file borrowed from:
# https://github.com/hatstand/symmetrical-octo-fiesta/blob/master/BUILD.opencv
# AND EXPANDED WITH:
# https://github.com/mjbots/bazel_deps/blob/master/tools/workspace/opencv/opencv.bzl
# AND THEN MODIFIED
package(default_visibility = ["//visibility:private"])

config_setting(
    name = "arm7",
    values = {
        "cpu": "armeabi-v7a",
    },
)

config_setting(
    name = "arm8",
    values = {
        "cpu": "aarch64-linux-gnu",
    },
)

# _OPENCV_COPTS = ["-D__OPENCV_BUILD"]
_OPENCV_COPTS = [
    "-D_USE_MATH_DEFINES",
    "-D__OPENCV_BUILD=1",
    "-D__STDC_CONSTANT_MACROS",
    "-D__STDC_FORMAT_MACROS",
    "-D__STDC_LIMIT_MACROS",
    "-I$(GENDIR)/external/opencv/private/",
]

cc_library(
    name = "opencv",
    visibility = ["//visibility:public"],
    deps = [
        ":_base_headers",
        ":opencv_core",
        ":opencv_highgui",
        ":opencv_imgcodecs",
        ":opencv_imgproc",
        ":opencv_ml",
        ":opencv_video",
        ":opencv_videoio",
    ],
)

cc_library(
    name = "_base_headers",
    hdrs = [
        "custom_hal.hpp",
        "cv_cpu_config.h",
        "cvconfig.h",
        "opencv2/opencv_modules.hpp",
    ],
    textual_hdrs = [
        "version_string.inc",
    ],
)

cc_library(
    name = "opencv_core",
    srcs = glob([
        "modules/core/src/**/*.cpp",
        "modules/core/src/**/*.hpp",
        "modules/core/include/**/*.hpp",
        "modules/core/include/**/*.h",
    ]) + [
        "custom_hal.hpp",
        "cvconfig.h",
        "opencl_kernels_core.hpp",
        "opencv2/opencv_modules.hpp",
        "version_string.inc",
    ],
    hdrs = ["modules/core/include/opencv2/core/opencl/ocl_defs.hpp"],
    copts = [
        "-Iexternal/zlib",
    ] + _OPENCV_COPTS,
    includes = [
        ".",
        "modules/core/include",
    ],
    linkopts = [
        "-ldl",
    ] + select({
        # ":arm": ["-llog"],
        "//conditions:default": ["-lpthread"],
    }),
    visibility = ["//visibility:public"],
    deps = [
        "//external:zlib",
    ],
)

genrule(
    name = "opencv_core_kernels",
    outs = ["opencl_kernels_core.hpp"],
    cmd = """
cat > $@ <<"EOF"
#include "opencv2/core/ocl.hpp"
#include "opencv2/core/ocl_genbase.hpp"
#include "opencv2/core/opencl/ocl_defs.hpp"
EOF""",
)

genrule(
    name = "opencv_video_kernels",
    outs = ["opencl_kernels_video.hpp"],
    cmd = """
cat > $@ <<"EOF"
// This file is auto-generated. Do not edit!

#include "opencv2/core/ocl.hpp"
#include "opencv2/core/ocl_genbase.hpp"
#include "opencv2/core/opencl/ocl_defs.hpp"

#ifdef HAVE_OPENCL

namespace cv
{
namespace ocl
{
namespace video
{

extern struct cv::ocl::internal::ProgramEntry bgfg_knn_oclsrc;
extern struct cv::ocl::internal::ProgramEntry bgfg_mog2_oclsrc;
extern struct cv::ocl::internal::ProgramEntry dis_flow_oclsrc;
extern struct cv::ocl::internal::ProgramEntry optical_flow_farneback_oclsrc;
extern struct cv::ocl::internal::ProgramEntry pyrlk_oclsrc;

}}}
#endif
EOF""",
)

genrule(
    name = "opencl_kernels_features2d",
    outs = ["opencl_kernels_features2d.hpp"],
    cmd = """
cat > $@ <<"EOF"
// This file is auto-generated. Do not edit!

#include "opencv2/core/ocl.hpp"
#include "opencv2/core/ocl_genbase.hpp"
#include "opencv2/core/opencl/ocl_defs.hpp"

#ifdef HAVE_OPENCL

namespace cv
{
namespace ocl
{
namespace features2d
{

extern const struct ProgramEntry brute_force_match;
extern ProgramSource brute_force_match_oclsrc;
extern const struct ProgramEntry fast;
extern ProgramSource fast_oclsrc;
extern const struct ProgramEntry orb;
extern ProgramSource orb_oclsrc;
}
}}
#endif
EOF""",
)

genrule(
    name = "opencl_kernels_calib3d",
    outs = ["opencl_kernels_calib3d.hpp"],
    cmd = """
cat > $@ <<"EOF"
// This file is auto-generated. Do not edit!

#include "opencv2/core/ocl.hpp"
#include "opencv2/core/ocl_genbase.hpp"
#include "opencv2/core/opencl/ocl_defs.hpp"

#ifdef HAVE_OPENCL

namespace cv
{
namespace ocl
{
namespace calib3d
{

extern const struct ProgramEntry stereobm;
extern ProgramSource stereobm_oclsrc;
}
}}
#endif
EOF""",
)


genrule(
    name = "cvconfig",
    srcs = select({
        ":arm7": ["cvconfig_armv7hf.h"],
        ":arm8": ["cvconfig_armv7hf.h"],
        "//conditions:default": ["cvconfig_amd64.h"],
    }),
    outs = ["cvconfig.h"],
    cmd = "cp $< $@",
)

genrule(
    name = "cv_cpu_config",
    srcs = select({
        ":arm7": ["cv_cpu_config_armv7hf.h"],
        ":arm8": ["cv_cpu_config_armv7hf.h"],
        "//conditions:default": ["cv_cpu_config_amd64.h"],
    }),
    outs = ["cv_cpu_config.h"],
    cmd = "cp $< $@",
)

# Copied from running the following on x86 machine. Note some things were
# manually commented out.
#   cmake .. && make -j6 && cat cvconfig.h
genrule(
    name = "cvconfig_amd64",
    outs = ["cvconfig_amd64.h"],
    cmd = """
cat > $@ <<"EOF"
#ifndef OPENCV_CVCONFIG_H_INCLUDED
#define OPENCV_CVCONFIG_H_INCLUDED

/* OpenCV compiled as static or dynamic libs */
#define BUILD_SHARED_LIBS

/* OpenCV intrinsics optimized code */
#define CV_ENABLE_INTRINSICS

/* OpenCV additional optimized code */
/* #undef CV_DISABLE_OPTIMIZATION */

/* Compile for 'real' NVIDIA GPU architectures */
#define CUDA_ARCH_BIN ""

/* Create PTX or BIN for 1.0 compute capability */
/* #undef CUDA_ARCH_BIN_OR_PTX_10 */

/* NVIDIA GPU features are used */
#define CUDA_ARCH_FEATURES ""

/* Compile for 'virtual' NVIDIA PTX architectures */
#define CUDA_ARCH_PTX ""

/* AVFoundation video libraries */
/* #undef HAVE_AVFOUNDATION */

/* V4L2 capturing support */
#define HAVE_CAMV4L2

/* AMD's Basic Linear Algebra Subprograms Library*/
/* #undef HAVE_CLAMDBLAS */

/* AMD's OpenCL Fast Fourier Transform Library*/
/* #undef HAVE_CLAMDFFT */

/* Clp support */
/* #undef HAVE_CLP */

/* Cocoa API */
/* #undef HAVE_COCOA */

/* NVIDIA CUDA Basic Linear Algebra Subprograms (BLAS) API*/
/* #undef HAVE_CUBLAS */

/* NVIDIA CUDA Runtime API*/
/* #undef HAVE_CUDA */

/* NVIDIA CUDA Fast Fourier Transform (FFT) API*/
/* #undef HAVE_CUFFT */

/* IEEE1394 capturing support */
/* #undef HAVE_DC1394 */

/* IEEE1394 capturing support - libdc1394 v2.x */
/* #undef HAVE_DC1394_2 */

/* DirectX */
/* #undef HAVE_DIRECTX */
/* #undef HAVE_DIRECTX_NV12 */
/* #undef HAVE_D3D11 */
/* #undef HAVE_D3D10 */
/* #undef HAVE_D3D9 */

/* DirectShow Video Capture library */
/* #undef HAVE_DSHOW */

/* Eigen Matrix & Linear Algebra Library */
/* #undef HAVE_EIGEN */

/* FFMpeg video library */
/* #undef HAVE_FFMPEG */

/* Geospatial Data Abstraction Library */
/* #undef HAVE_GDAL */

/* GStreamer multimedia framework */
/* #undef HAVE_GSTREAMER */

/* GTK+ 2.0 Thread support */
/* #undef HAVE_GTHREAD */

/* GTK+ 2.x toolkit */
/* #undef HAVE_GTK */

/* Halide support */
/* #undef HAVE_HALIDE */

/* Vulkan support */
/* #undef HAVE_VULKAN */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Intel Perceptual Computing SDK library */
/* #undef HAVE_INTELPERC */

// MURTIS commented this out...
/* Intel Integrated Performance Primitives */
//#define HAVE_IPP
//#define HAVE_IPP_ICV
//#define HAVE_IPP_IW
//#define HAVE_IPP_IW_LL

// MURTIS commented this out...
/* JPEG-2000 codec */
//#define HAVE_JASPER

/* IJG JPEG codec */
#define HAVE_JPEG

/* libpng/png.h needs to be included */
#define HAVE_LIBPNG_PNG_H

/* GDCM DICOM codec */
/* #undef HAVE_GDCM */

/* Microsoft Media Foundation Capture library */
/* #undef HAVE_MSMF */

/* NVIDIA Video Decoding API*/
/* #undef HAVE_NVCUVID */

/* NVIDIA Video Encoding API*/
/* #undef HAVE_NVCUVENC */

/* OpenCL Support */
// MURTIS commented this out...
//#define HAVE_OPENCL
/* #undef HAVE_OPENCL_STATIC */
/* #undef HAVE_OPENCL_SVM */

/* OpenEXR codec */
// MURTIS commented this out...
//#define HAVE_OPENEXR

/* OpenGL support*/
/* #undef HAVE_OPENGL */

/* OpenNI library */
/* #undef HAVE_OPENNI */

/* OpenNI library */
/* #undef HAVE_OPENNI2 */

/* librealsense library */
/* #undef HAVE_LIBREALSENSE */

/* PNG codec */
#define HAVE_PNG

/* Posix threads (pthreads) */
#define HAVE_PTHREAD

/* parallel_for with pthreads */
#define HAVE_PTHREADS_PF

/* Qt support */
/* #undef HAVE_QT */

/* Qt OpenGL support */
/* #undef HAVE_QT_OPENGL */

/* Intel Threading Building Blocks */
/* #undef HAVE_TBB */

/* Ste||ar Group High Performance ParallelX */
/* #undef HAVE_HPX */

/* TIFF codec */
// MURTIS commented this out...
//#define HAVE_TIFF

/* V4L2 capturing support in videoio.h */
/* #undef HAVE_VIDEOIO */

/* Win32 UI */
/* #undef HAVE_WIN32UI */

/* XIMEA camera support */
/* #undef HAVE_XIMEA */

/* Xine video library */
/* #undef HAVE_XINE */

/* Define if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
/* #undef WORDS_BIGENDIAN */

/* gPhoto2 library */
/* #undef HAVE_GPHOTO2 */

/* VA library (libva) */
/* #undef HAVE_VA */

/* Intel VA-API/OpenCL */
/* #undef HAVE_VA_INTEL */

/* Intel Media SDK */
/* #undef HAVE_MFX */

/* Lapack */
/* #undef HAVE_LAPACK */

/* Library was compiled with functions instrumentation */
/* #undef ENABLE_INSTRUMENTATION */

/* OpenVX */
/* #undef HAVE_OPENVX */

#if defined(HAVE_XINE)         || \
    defined(HAVE_GSTREAMER)    || \
    defined(HAVE_AVFOUNDATION) || \
    /*defined(HAVE_OPENNI)     || too specialized */ \
    defined(HAVE_FFMPEG)       || \
    defined(HAVE_MSMF)
#define HAVE_VIDEO_INPUT
#endif

#if /*defined(HAVE_XINE)       || */\
    defined(HAVE_GSTREAMER)    || \
    defined(HAVE_AVFOUNDATION) || \
    defined(HAVE_FFMPEG)       || \
    defined(HAVE_MSMF)
#define HAVE_VIDEO_OUTPUT
#endif

/* OpenCV trace utilities */
#define OPENCV_TRACE

/* Library QR-code decoding */
#define HAVE_QUIRC

#endif // OPENCV_CVCONFIG_H_INCLUDED

EOF""",
)

# TODO(murtis) get this info
genrule(
    name = "cvconfig_armv7hf",
    outs = ["cvconfig_armv7hf.h"],
    cmd = """
cat > $@ <<"EOF"
#ifndef OPENCV_CVCONFIG_H_INCLUDED
#define OPENCV_CVCONFIG_H_INCLUDED

/* OpenCV compiled as static or dynamic libs */
#define BUILD_SHARED_LIBS

/* OpenCV intrinsics optimized code */
#define CV_ENABLE_INTRINSICS

/* OpenCV additional optimized code */
/* #undef CV_DISABLE_OPTIMIZATION */

/* Compile for 'real' NVIDIA GPU architectures */
#define CUDA_ARCH_BIN ""

/* Create PTX or BIN for 1.0 compute capability */
/* #undef CUDA_ARCH_BIN_OR_PTX_10 */

/* NVIDIA GPU features are used */
#define CUDA_ARCH_FEATURES ""

/* Compile for 'virtual' NVIDIA PTX architectures */
#define CUDA_ARCH_PTX ""

/* AVFoundation video libraries */
/* #undef HAVE_AVFOUNDATION */

/* V4L2 capturing support */
#define HAVE_CAMV4L2

/* AMD's Basic Linear Algebra Subprograms Library*/
/* #undef HAVE_CLAMDBLAS */

/* AMD's OpenCL Fast Fourier Transform Library*/
/* #undef HAVE_CLAMDFFT */

/* Clp support */
/* #undef HAVE_CLP */

/* Cocoa API */
/* #undef HAVE_COCOA */

/* NVIDIA CUDA Basic Linear Algebra Subprograms (BLAS) API*/
/* #undef HAVE_CUBLAS */

/* NVIDIA CUDA Runtime API*/
/* #undef HAVE_CUDA */

/* NVIDIA CUDA Fast Fourier Transform (FFT) API*/
/* #undef HAVE_CUFFT */

/* IEEE1394 capturing support */
/* #undef HAVE_DC1394 */

/* IEEE1394 capturing support - libdc1394 v2.x */
/* #undef HAVE_DC1394_2 */

/* DirectX */
/* #undef HAVE_DIRECTX */
/* #undef HAVE_DIRECTX_NV12 */
/* #undef HAVE_D3D11 */
/* #undef HAVE_D3D10 */
/* #undef HAVE_D3D9 */

/* DirectShow Video Capture library */
/* #undef HAVE_DSHOW */

/* Eigen Matrix & Linear Algebra Library */
/* #undef HAVE_EIGEN */

/* FFMpeg video library */
/* #undef HAVE_FFMPEG */

/* Geospatial Data Abstraction Library */
/* #undef HAVE_GDAL */

/* GStreamer multimedia framework */
/* #undef HAVE_GSTREAMER */

/* GTK+ 2.0 Thread support */
/* #undef HAVE_GTHREAD */

/* GTK+ 2.x toolkit */
/* #undef HAVE_GTK */

/* Halide support */
/* #undef HAVE_HALIDE */

/* Vulkan support */
/* #undef HAVE_VULKAN */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Intel Perceptual Computing SDK library */
/* #undef HAVE_INTELPERC */

/* Intel Integrated Performance Primitives */
/* #undef HAVE_IPP */
/* #undef HAVE_IPP_ICV */
/* #undef HAVE_IPP_IW */
/* #undef HAVE_IPP_IW_LL */

/* JPEG-2000 codec */
//#define HAVE_JASPER

/* IJG JPEG codec */
#define HAVE_JPEG

/* libpng/png.h needs to be included */
/* #undef HAVE_LIBPNG_PNG_H */

/* GDCM DICOM codec */
/* #undef HAVE_GDCM */

/* Microsoft Media Foundation Capture library */
/* #undef HAVE_MSMF */

/* NVIDIA Video Decoding API*/
/* #undef HAVE_NVCUVID */

/* NVIDIA Video Encoding API*/
/* #undef HAVE_NVCUVENC */

/* OpenCL Support */
//#define HAVE_OPENCL
/* #undef HAVE_OPENCL_STATIC */
/* #undef HAVE_OPENCL_SVM */

/* OpenEXR codec */
//#define HAVE_OPENEXR

/* OpenGL support*/
/* #undef HAVE_OPENGL */

/* OpenNI library */
/* #undef HAVE_OPENNI */

/* OpenNI library */
/* #undef HAVE_OPENNI2 */

/* librealsense library */
/* #undef HAVE_LIBREALSENSE */

/* PNG codec */
//#define HAVE_PNG

/* Posix threads (pthreads) */
#define HAVE_PTHREAD

/* parallel_for with pthreads */
#define HAVE_PTHREADS_PF

/* Qt support */
/* #undef HAVE_QT */

/* Qt OpenGL support */
/* #undef HAVE_QT_OPENGL */

/* Intel Threading Building Blocks */
/* #undef HAVE_TBB */

/* Ste||ar Group High Performance ParallelX */
/* #undef HAVE_HPX */

/* TIFF codec */
//#define HAVE_TIFF

/* V4L2 capturing support in videoio.h */
/* #undef HAVE_VIDEOIO */

/* Win32 UI */
/* #undef HAVE_WIN32UI */

/* XIMEA camera support */
/* #undef HAVE_XIMEA */

/* Xine video library */
/* #undef HAVE_XINE */

/* Define if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
/* #undef WORDS_BIGENDIAN */

/* gPhoto2 library */
/* #undef HAVE_GPHOTO2 */

/* VA library (libva) */
/* #undef HAVE_VA */

/* Intel VA-API/OpenCL */
/* #undef HAVE_VA_INTEL */

/* Intel Media SDK */
/* #undef HAVE_MFX */

/* Lapack */
/* #undef HAVE_LAPACK */

/* Library was compiled with functions instrumentation */
/* #undef ENABLE_INSTRUMENTATION */

/* OpenVX */
/* #undef HAVE_OPENVX */

#if defined(HAVE_XINE)         || \
    defined(HAVE_GSTREAMER)    || \
    defined(HAVE_AVFOUNDATION) || \
    /*defined(HAVE_OPENNI)     || too specialized */ \
    defined(HAVE_FFMPEG)       || \
    defined(HAVE_MSMF)
#define HAVE_VIDEO_INPUT
#endif

#if /*defined(HAVE_XINE)       || */\
    defined(HAVE_GSTREAMER)    || \
    defined(HAVE_AVFOUNDATION) || \
    defined(HAVE_FFMPEG)       || \
    defined(HAVE_MSMF)
#define HAVE_VIDEO_OUTPUT
#endif

/* OpenCV trace utilities */
#define OPENCV_TRACE

/* Library QR-code decoding */
#define HAVE_QUIRC

#endif // OPENCV_CVCONFIG_H_INCLUDED

EOF""",
)

# TODO(murtis) get this info
genrule(
    name = "cv_cpu_config_armv7hf",
    outs = ["cv_cpu_config_armv7hf.h"],
    cmd = """
cat > $@ <<"EOF"
// OpenCV CPU baseline features
#define CV_CPU_BASELINE_FEATURES 0 \
// OpenCV supported CPU dispatched features
#define CV_CPU_DISPATCH_FEATURES 0 \

EOF""",
)

# Copied from running the following on x86 machine. Note some things were
# manually commented out.
#   cmake .. && make -j6 && cat cv_cpu_config.h
genrule(
    name = "cv_cpu_config_amd64",
    outs = ["cv_cpu_config_amd64.h"],
    cmd = """
cat > $@ <<"EOF"
// OpenCV CPU baseline features

#define CV_CPU_COMPILE_SSE 1
#define CV_CPU_BASELINE_COMPILE_SSE 1

#define CV_CPU_COMPILE_SSE2 1
#define CV_CPU_BASELINE_COMPILE_SSE2 1

#define CV_CPU_COMPILE_SSE3 1
#define CV_CPU_BASELINE_COMPILE_SSE3 1

#define CV_CPU_BASELINE_FEATURES 0 \
    , CV_CPU_SSE \
    , CV_CPU_SSE2 \
    , CV_CPU_SSE3 \


// OpenCV supported CPU dispatched features

#define CV_CPU_DISPATCH_COMPILE_SSE4_1 1
#define CV_CPU_DISPATCH_COMPILE_SSE4_2 1
#define CV_CPU_DISPATCH_COMPILE_FP16 1
#define CV_CPU_DISPATCH_COMPILE_AVX 1
#define CV_CPU_DISPATCH_COMPILE_AVX2 1
#define CV_CPU_DISPATCH_COMPILE_AVX512_SKX 1


#define CV_CPU_DISPATCH_FEATURES 0 \
    , CV_CPU_SSE4_1 \
    , CV_CPU_SSE4_2 \
    , CV_CPU_FP16 \
    , CV_CPU_AVX \
    , CV_CPU_AVX2 \
    , CV_CPU_AVX512_SKX \

EOF""",
)

genrule(
    name = "custom_hal",
    outs = ["custom_hal.hpp"],
    cmd = """
cat > $@ <<"EOF"
#ifndef _CUSTOM_HAL_INCLUDED_
#define _CUSTOM_HAL_INCLUDED_

#endif
EOF""",
)

genrule(
    name = "version_string",
    outs = ["version_string.inc"],
    cmd = "echo '\"OpenCV 3.1.0\"' > $@",
)

genrule(
    name = "opencv_modules",
    outs = ["opencv2/opencv_modules.hpp"],
    cmd = """
cat > $@ <<"EOF"
#define HAVE_OPENCV_CORE
#define HAVE_OPENCV_IMGCODECS
#define HAVE_OPENCV_IMGPROC
#define HAVE_OPENCV_ML
#define HAVE_OPENCV_VIDEOIO
EOF""",
)

cc_library(
    name = "opencv_imgproc",
    srcs = glob([
        "modules/imgproc/src/**/*.cpp",
        "modules/imgproc/src/**/*.hpp",
        "modules/imgproc/src/**/*.h",
        "modules/imgproc/include/**/*.hpp",
        "modules/imgproc/include/**/*.h",
    ]) + ["opencl_kernels_imgproc.hpp"],
    copts = _OPENCV_COPTS,
    includes = [
        ".",
        "modules/core/include",
        "modules/imgproc/include",
    ],
    visibility = ["//visibility:public"],
    deps = [":opencv_core"],
)

genrule(
    name = "opencv_imgproc_kernels",
    outs = ["opencl_kernels_imgproc.hpp"],
    cmd = """
cat > $@ <<"EOF"
#include "opencv2/core/ocl.hpp"
#include "opencv2/core/ocl_genbase.hpp"
#include "opencv2/core/opencl/ocl_defs.hpp"
EOF""",
)

cc_library(
    name = "opencv_ml",
    srcs = glob([
        "modules/ml/src/**/*.cpp",
        "modules/ml/src/**/*.hpp",
        "modules/ml/include/**/*.hpp",
    ]),
    copts = _OPENCV_COPTS,
    includes = ["modules/ml/include"],
    visibility = ["//visibility:public"],
    deps = [":opencv_core"],
)

cc_library(
    name = "opencv_highgui",
    srcs = glob(
        [
            "modules/highgui/src/**/*.cpp",
            "modules/highgui/src/**/*.hpp",
            "modules/highgui/include/**/*.hpp",
            "modules/highgui/include/**/*.h",
        ],
        exclude = [
            "modules/highgui/src/window_winrt_bridge.cpp",
            "modules/highgui/src/window_carbon.cpp",
            "modules/highgui/src/window_winrt.cpp",
            "modules/highgui/src/window_w32.cpp",
            "modules/highgui/src/window_QT.cpp",
            "modules/highgui/src/window_gtk.cpp",
        ],
    ),
    hdrs = ["modules/highgui/include/opencv2/highgui.hpp"],
    copts = _OPENCV_COPTS,
    includes = ["modules/highgui/include"],
    visibility = ["//visibility:public"],
    deps = [
        ":opencv_core",
        ":opencv_imgcodecs",
        ":opencv_videoio",
    ],
)

cc_library(
    name = "opencv_imgcodecs",
    srcs = glob([
        "modules/imgcodecs/src/**/*.cpp",
        "modules/imgcodecs/src/**/*.hpp",
        "modules/imgcodecs/include/**/*.hpp",
        "modules/imgcodecs/include/**/*.h",
    ]),
    copts = [
        "-Iexternal/libpng_archive",
        "-Iexternal/zlib",
    ] + _OPENCV_COPTS,
    includes = [
        "modules/imgcodecs/include",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":opencv_core",
        ":opencv_imgproc",
        "//external:png",
        "//external:zlib",
        "@libjpeg_archive//:libjpeg",
    ],
)

cc_library(
    name = "opencv_videoio",
    srcs = glob(
        [
            "modules/videoio/src/**/*.cpp",
            "modules/videoio/src/**/*.hpp",
            "modules/videoio/include/**/*.hpp",
            "modules/videoio/include/**/*.h",
        ],
        exclude = [
            "modules/videoio/src/cap_giganetix.cpp",
            "modules/videoio/src/cap_gstreamer.cpp",
            "modules/videoio/src/cap_qt.cpp",
            "modules/videoio/src/cap_unicap.cpp",
            "modules/videoio/src/cap_vfw.cpp",
            "modules/videoio/src/cap_winrt/**/*",
            "modules/videoio/src/cap_winrt_bridge.cpp",
            "modules/videoio/src/cap_winrt_capture.cpp",
            "modules/videoio/src/cap_winrt_video.cpp",
            "modules/videoio/src/cap_ximea.cpp",
            "modules/videoio/src/cap_xine.cpp",
        ],
    ),
    copts = _OPENCV_COPTS,
    includes = ["modules/videoio/include"],
    visibility = ["//visibility:public"],
    deps = [
        ":opencv_core",
        ":opencv_imgcodecs",
    ],
)

cc_library(
    name = "opencv_video",
    srcs = glob(
        [
            "modules/video/src/**/*.cpp",
            "modules/video/src/**/*.hpp",
            "modules/video/include/**/*.hpp",
            "modules/video/include/**/*.h",
        ],
    ),
    hdrs = [
        "opencl_kernels_video.hpp",
    ],
    copts = _OPENCV_COPTS,
    includes = [
        "modules/video/include",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":opencv_core",
        ":opencv_imgproc",
    ],
)


cc_library(
    name = "opencv_flann",
    srcs = glob(
        [
            "modules/flann/src/**/*.cpp",
            "modules/flann/src/**/*.hpp",
            "modules/flann/include/**/*.hpp",
            "modules/flann/include/**/*.h",
        ],
    ),
    hdrs = [
        # "opencl_kernels_video.hpp",
    ],
    copts = _OPENCV_COPTS,
    includes = [
        "modules/flann/include",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":opencv_core",
    ],
)

cc_library(
    name = "opencv_features2d",
    srcs = glob(
        [
            "modules/features2d/src/**/*.cpp",
            "modules/features2d/src/**/*.hpp",
            "modules/features2d/src/**/*.h",
            "modules/features2d/include/**/*.hpp",
            "modules/features2d/include/**/*.h",
        ],
    ),
    hdrs = [
        "opencl_kernels_features2d.hpp",
    ],
    copts = _OPENCV_COPTS,
    includes = [
        "modules/features2d/include",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":opencv_core",
        ":opencv_imgproc",
        ":opencv_highgui",
        ":opencv_flann",
    ],
)


cc_library(
    name = "opencv_calib3d",
    srcs = glob(
        [
            "modules/calib3d/src/**/*.cpp",
            "modules/calib3d/src/**/*.hpp",
            "modules/calib3d/src/**/*.h",
            "modules/calib3d/include/**/*.hpp",
            "modules/calib3d/include/**/*.h",
            "modules/calib3d/src/precomp.hpp",
        ],
    ),
    hdrs = [
        "opencl_kernels_calib3d.hpp",
        # "modules/calib3d/src/precomp.hpp",
    ],
    copts = _OPENCV_COPTS,
    includes = [
        "modules/calib3d/include",
        "modules/calib3d/src",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":opencv_core",
        ":opencv_imgproc",
        ":opencv_highgui",
        ":opencv_features2d",
    ],
)
