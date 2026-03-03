cmake_minimum_required(VERSION 3.17)

project(RibnXtr2023 LANGUAGES C CXX)

# ---- MSVC / build quality ----
if(MSVC)
  add_compile_options(/MP)
endif()

# ---- Qt ----
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Prefer explicit Qt5_DIR; fallback to QTDIR if set
if(DEFINED ENV{QTDIR} AND NOT DEFINED Qt5_DIR)
  list(APPEND CMAKE_PREFIX_PATH "$ENV{QTDIR}")
endif()

find_package(Qt5 REQUIRED COMPONENTS
  Core Gui Widgets Network Multimedia MultimediaWidgets
)

# ---- OpenMP ----
find_package(OpenMP REQUIRED)

# ---- Kinect (custom Find-module) ----
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")
find_package(KinectSDK2 REQUIRED)

# ---- OpenCV ----
find_package(OpenCV REQUIRED)

# ---- VTK ----
if(NOT VTK_DIR AND DEFINED ENV{VTK_DIR})
  set(VTK_DIR "$ENV{VTK_DIR}")
endif()
find_package(VTK REQUIRED)

# ---- ITK ----
if(NOT ITK_DIR AND DEFINED ENV{ITK_DIR})
  set(ITK_DIR "$ENV{ITK_DIR}")
endif()
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

# ---- Armadillo ----
find_package(Armadillo REQUIRED)

# ---- Our code ----
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

# ---- Local deps from deps.cmake (QtXlsx / FFmpeg) ----
# (muszą być ustawione w deps.cmake)
if(NOT DEFINED QTXLSX_INCLUDE_DIR)
  message(FATAL_ERROR "QTXLSX_INCLUDE_DIR not set (set it in deps.cmake)")
endif()
if(NOT DEFINED QTXLSX_LIB)
  message(FATAL_ERROR "QTXLSX_LIB not set (set it in deps.cmake)")
endif()

# FFmpeg jest opcjonalny jeśli używasz
# Linkujemy tylko gdy podane są wszystkie zmienne FFmpeg.
set(_FFMPEG_READY FALSE)
if(DEFINED FFMPEG_INCLUDE_DIR
   AND DEFINED FFMPEG_AVCODEC_LIB
   AND DEFINED FFMPEG_AVFORMAT_LIB
   AND DEFINED FFMPEG_SWSCALE_LIB
   AND DEFINED FFMPEG_AVUTIL_LIB)
  set(_FFMPEG_READY TRUE)
elseif(DEFINED FFMPEG_INCLUDE_DIR
       OR DEFINED FFMPEG_AVCODEC_LIB
       OR DEFINED FFMPEG_AVFORMAT_LIB
       OR DEFINED FFMPEG_SWSCALE_LIB
       OR DEFINED FFMPEG_AVUTIL_LIB)
  message(WARNING "Partial FFmpeg configuration detected; define all FFMPEG_* vars or none. FFmpeg linkage will be skipped.")
endif()

target_include_directories(RibnXtr2023 PRIVATE
  "${INCLUDE_DIR}"
  "${SOURCE_DIR}"
  "${OpenCV_INCLUDE_DIRS}"
  "${ARMADILLO_INCLUDE_DIRS}"
  "${QTXLSX_INCLUDE_DIR}"
  ${KinectSDK2_INCLUDE_DIRS}
)

if(_FFMPEG_READY)
  target_include_directories(RibnXtr2023 PRIVATE "${FFMPEG_INCLUDE_DIR}")
endif()

target_link_libraries(RibnXtr2023 PRIVATE
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets
  Qt5::Network
  Qt5::Multimedia
  Qt5::MultimediaWidgets

  ${VTK_LIBRARIES}
  ${ITK_LIBRARIES}
  ${ARMADILLO_LIBRARIES}
  ${OpenCV_LIBS}

  KinectSDK2::KinectSDK2
  OpenMP::OpenMP_CXX

  "${QTXLSX_LIB}"
  Iphlpapi.lib
)

if(_FFMPEG_READY)
  target_link_libraries(RibnXtr2023 PRIVATE
    "${FFMPEG_AVCODEC_LIB}"
    "${FFMPEG_AVFORMAT_LIB}"
    "${FFMPEG_SWSCALE_LIB}"
    "${FFMPEG_AVUTIL_LIB}"
  )
endif()

# ---- MSVC runtime/run dir ----
if(MSVC)
  set_property(TARGET RibnXtr2023 PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${RUN_DIR}")
  set_target_properties(RibnXtr2023 PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
endif()