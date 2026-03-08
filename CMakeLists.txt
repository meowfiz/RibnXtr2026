cmake_minimum_required(VERSION 3.17)

project(RibnXtr2023 LANGUAGES C CXX)

# ---- Qt automoc/uic ----
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Prefer explicit Qt5_DIR from deps.cmake; fallback to QTDIR env if set
if(DEFINED ENV{QTDIR} AND NOT DEFINED Qt5_DIR)
  list(APPEND CMAKE_PREFIX_PATH "$ENV{QTDIR}")
endif()

# ---- MSVC: multiprocessor compilation ----
if(MSVC)
  add_compile_options(/MP)
endif()

# ---- Qt ----
find_package(Qt5 REQUIRED COMPONENTS
  Core Gui Widgets Network Multimedia MultimediaWidgets
)

# ---- OpenMP ----
find_package(OpenMP REQUIRED)

# ---- Kinect SDK v2 (custom Find-module from repo) ----
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")
find_package(KinectSDK2 REQUIRED)

# ---- OpenCV ----
find_package(OpenCV REQUIRED)

# ---- ITK ----
# IMPORTANT: DO NOT call find_package(VTK) here.
# TubeTK/ITK may pull VTK internally and double-import causes target conflicts.
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

# ---- Armadillo ----
find_package(Armadillo REQUIRED)

# ---- Our code paths ----
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/project_files/RibnXtr2022-project_files/ribnxtr2_Files/src/include")
set(SOURCE_DIR  "${CMAKE_SOURCE_DIR}/project_files/RibnXtr2022-project_files/ribnxtr2_Files/src/source")
set(RUN_DIR     "${CMAKE_SOURCE_DIR}/project_files/RibnXtr2022-project_files/ribnxtr2_Files/run_files")

file(GLOB_RECURSE SOURCES
  "${INCLUDE_DIR}/*.h"
  "${SOURCE_DIR}/*.c"
  "${SOURCE_DIR}/*.cpp"
)

# ---- Target ----
if(WIN32)
  add_executable(RibnXtr2023 WIN32 ${SOURCES})
else()
  add_executable(RibnXtr2023 ${SOURCES})
endif()

# ---- Local deps from deps.cmake (required) ----
if(NOT DEFINED QTXLSX_INCLUDE_DIR)
  message(FATAL_ERROR "QTXLSX_INCLUDE_DIR not set (set it in deps.cmake)")
endif()
if(NOT DEFINED QTXLSX_LIB)
  message(FATAL_ERROR "QTXLSX_LIB not set (set it in deps.cmake)")
endif()

# FFmpeg: jeśli używasz w kodzie, ustaw w deps.cmake; jeśli nie, zostaw puste
# (CMake przełknie puste stringi w linkowaniu, ale lepiej mieć ustawione)
if(NOT DEFINED FFMPEG_INCLUDE_DIR)
  set(FFMPEG_INCLUDE_DIR "" CACHE PATH "")
endif()
if(NOT DEFINED FFMPEG_AVCODEC_LIB)
  set(FFMPEG_AVCODEC_LIB "" CACHE FILEPATH "")
  set(FFMPEG_AVFORMAT_LIB "" CACHE FILEPATH "")
  set(FFMPEG_SWSCALE_LIB "" CACHE FILEPATH "")
  set(FFMPEG_AVUTIL_LIB "" CACHE FILEPATH "")
endif()

# ---- Includes (target-scoped) ----
target_include_directories(RibnXtr2023 PRIVATE
  "${INCLUDE_DIR}"
  "${SOURCE_DIR}"

  "${OpenCV_INCLUDE_DIRS}"
  "${ARMADILLO_INCLUDE_DIRS}"

  "${QTXLSX_INCLUDE_DIR}"
  "${FFMPEG_INCLUDE_DIR}"

  ${KinectSDK2_INCLUDE_DIRS}
)

# ---- Link ----
target_link_libraries(RibnXtr2023 PRIVATE
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets
  Qt5::Network
  Qt5::Multimedia
  Qt5::MultimediaWidgets

  ${ITK_LIBRARIES}
  ${ARMADILLO_LIBRARIES}
  ${OpenCV_LIBS}

  KinectSDK2::KinectSDK2
  OpenMP::OpenMP_CXX

  "${QTXLSX_LIB}"
  Iphlpapi.lib

  "${FFMPEG_AVCODEC_LIB}"
  "${FFMPEG_AVFORMAT_LIB}"
  "${FFMPEG_SWSCALE_LIB}"
  "${FFMPEG_AVUTIL_LIB}"
)

# ---- VS debugger working dir ----
if(MSVC)
  set_property(TARGET RibnXtr2023 PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${RUN_DIR}")
endif()