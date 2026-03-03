cmake_minimum_required(VERSION 3.17)
# Force MSBuild-style macro out of Ninja files (legacy ITK/VTK exports)
set(CMAKE_MSVCIDE_RUN_PATH "")
string(REPLACE "$(Configuration)" "Debug" CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
string(REPLACE "$(Configuration)" "Debug" CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")
string(REPLACE "$(Configuration)" "Debug" CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS}")
project(RibnXtr2023)
set(CMAKE_C_USE_RESPONSE_FILE_FOR_INCLUDES 1)
set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES 1)
set(CMAKE_NINJA_FORCE_RESPONSE_FILE ON)
# ======================= Qt ======================= #
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Prefer explicit Qt5_DIR from configureSettings; fallback to QTDIR if set
if(DEFINED ENV{QTDIR} AND NOT DEFINED Qt5_DIR)
  list(APPEND CMAKE_PREFIX_PATH "$ENV{QTDIR}")
endif()

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(Qt5MultimediaWidgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets REQUIRED)

# ======================= Kinect SDK v2 ======================= #
# Use our custom FindKinectSDK2.cmake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")
find_package(KinectSDK2 REQUIRED)

# ======================= OpenCV ======================= #
find_package(OpenCV REQUIRED)
# (no global include_directories; attach to target later)

# ======================= VTK ======================= #
# Prefer VTK_DIR from configureSettings; fallback to env
if(NOT VTK_DIR AND DEFINED ENV{VTK_DIR})
  set(VTK_DIR "$ENV{VTK_DIR}")
endif()
find_package(VTK REQUIRED)

# ======================= ITK ======================= #
if(NOT ITK_DIR AND DEFINED ENV{ITK_DIR})
  set(ITK_DIR "$ENV{ITK_DIR}")
endif()
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

# ======================= Armadillo ======================= #
find_package(Armadillo REQUIRED)

# ======================= Extra includes ======================= #
include_directories("C:/tools/QtXlsxWriter-master/include/QtXlsx")
include_directories("c:/tools/ffmpeg/include")

# ======================= Our code ======================= #
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/project_files/RibnXtr2022-project_files/ribnxtr2_Files/src/include")
set(SOURCE_DIR  "${CMAKE_SOURCE_DIR}/project_files/RibnXtr2022-project_files/ribnxtr2_Files/src/source")
set(RUN_DIR     "${CMAKE_SOURCE_DIR}/project_files/RibnXtr2022-project_files/ribnxtr2_Files/run_files")

file(GLOB_RECURSE SOURCES
  "${INCLUDE_DIR}/*.h"
  "${SOURCE_DIR}/*.c"
  "${SOURCE_DIR}/*.cpp"
)

# ======================= Target ======================= #
if(WIN32)
  add_executable(RibnXtr2023 WIN32 ${SOURCES})
else()
  add_executable(RibnXtr2023 ${SOURCES})
endif()

# Includes attached to target (cleaner than global include_directories)
target_include_directories(RibnXtr2023 PRIVATE
  "${INCLUDE_DIR}"
  "${SOURCE_DIR}"
  "${OpenCV_INCLUDE_DIRS}"
  "${ARMADILLO_INCLUDE_DIRS}"
)

# Kinect include dirs from find-module / config
if(KinectSDK2_FOUND)
  target_include_directories(RibnXtr2023 PRIVATE ${KinectSDK2_INCLUDE_DIRS})
endif()

# ======================= Linking (ALL keyword signature) ======================= #
target_link_libraries(RibnXtr2023 PRIVATE
  Qt5::Multimedia
  Qt5::MultimediaWidgets
  Qt5::Network
  Qt5::Widgets

  ${VTK_LIBRARIES}
  ${ITK_LIBRARIES}
  ${ARMADILLO_LIBRARIES}
  ${OpenCV_LIBS}

  KinectSDK2::KinectSDK2
)

set(MY_LIB_PACK
  "C:/tools/QtXlsxWriter-master/lib/Qt5Xlsx.lib"
  Iphlpapi.lib
  "c:/tools/ffmpeg/lib/avcodec.lib"
  "c:/tools/ffmpeg/lib/avformat.lib"
  "c:/tools/ffmpeg/lib/swscale.lib"
  "c:/tools/ffmpeg/lib/avutil.lib"
)

target_link_libraries(RibnXtr2023 PRIVATE ${MY_LIB_PACK})

# ======================= Fix: bad add_custom_command ======================= #
# The previous line:
#   add_custom_command(TARGET RibnXtr2023 POST_BUILD ${KinectSDK2_COMMANDS})
# is invalid unless it contains proper COMMAND keywords.
# Most projects don't need this at all, so we remove it.
#
# If you *actually* need to run extra Kinect post-build commands, use a safe template:
# add_custom_command(TARGET RibnXtr2023 POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E echo "Post-build step"
# )

# ======================= MSVC tweaks ======================= #
if(MSVC)
  target_compile_options(RibnXtr2023 PRIVATE "/MP")

  # Keep VS debugger working dir (Ninja ignores this; VS uses it)
  set_property(TARGET RibnXtr2023 PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${RUN_DIR}")

  # Console subsystem (kept from your original intent)
  set_target_properties(RibnXtr2023 PROPERTIES
    LINK_FLAGS "/SUBSYSTEM:CONSOLE"
  )
endif()