cmake_minimum_required(VERSION 3.14)
project(ImageRegionSearch)
set(CMAKE_GENERATOR_PLATFORM x64)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(BUILD_APPS "build apps" OFF)
option(BUILD_C_INTERFACE "build c interface" ON)
option(BUILD_SHARED_LIBS "build shared library" ON)

if(BUILD_APPS)
    set(BUILD_C_INTERFACE ON)
endif()

find_package(OpenCV REQUIRED)
set(Boost_INCLUDE_DIR $ENV{BOOST_ROOT})
#find_package(Boost REQUIRED)

include_directories(include extern)
add_subdirectory(src)

if(BUILD_C_INTERFACE)
    add_subdirectory(bindings/c)
endif()

if(BUILD_APPS)
    add_subdirectory(apps)
endif()

# add_subdirectory(bindings/c)
# add_subdirectory(apps)