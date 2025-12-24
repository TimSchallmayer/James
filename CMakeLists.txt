cmake_minimum_required(VERSION 3.20)
project(James)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

add_executable(${PROJECT_NAME} main.cpp)
find_package(portaudio CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE portaudio)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/libs/vosk)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/libs/vosk/libvosk.lib)