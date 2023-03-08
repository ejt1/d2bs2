set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(VCPKG_HOST_TRIPLET "x86-windows-static" CACHE STRING "")
set(VCPKG_TARGET_TRIPLET "x86-windows-static" CACHE STRING "")

# Set default built to release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()

# Make sure debug/release defines
if(CMAKE_BUILD_TYPE MATCHES "^(D|d)ebug$")
  add_definitions(-DDEBUG -D_DEBUG)
else()
  add_definitions(-DNDEBUG -D_NDEBUG)
endif()

# Set C++ standard
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 23)
  set(CMAKE_CXX_STANDARD_REQUIRED True)
endif()

# Extract d2bs version from d2bs/src/version.h
include(cmake/d2bs_version.cmake)
d2bs_extract_version()
