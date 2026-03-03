# FindKinectSDK2.cmake
# Minimal finder for Microsoft Kinect for Windows SDK v2.0

set(_KINECT_ROOT
    "C:/Program Files/Microsoft SDKs/Kinect/v2.0_1409"
    CACHE PATH "Kinect SDK2 root"
)

# Headers
find_path(KinectSDK2_INCLUDE_DIR
  NAMES Kinect.h
  PATHS "${_KINECT_ROOT}/inc"
  NO_DEFAULT_PATH
)

# Libs (x64)
find_library(KinectSDK2_LIBRARY
  NAMES Kinect20
  PATHS "${_KINECT_ROOT}/Lib/x64"
  NO_DEFAULT_PATH
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(KinectSDK2
  REQUIRED_VARS KinectSDK2_INCLUDE_DIR KinectSDK2_LIBRARY
)

if(KinectSDK2_FOUND)
  set(KinectSDK2_LIBRARIES "${KinectSDK2_LIBRARY}")
  set(KinectSDK2_INCLUDE_DIRS "${KinectSDK2_INCLUDE_DIR}")

  if(NOT TARGET KinectSDK2::KinectSDK2)
    add_library(KinectSDK2::KinectSDK2 UNKNOWN IMPORTED)
    set_target_properties(KinectSDK2::KinectSDK2 PROPERTIES
      IMPORTED_LOCATION "${KinectSDK2_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${KinectSDK2_INCLUDE_DIR}"
    )
  endif()
endif()