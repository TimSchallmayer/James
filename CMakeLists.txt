cmake_minimum_required(VERSION 3.20)

# Toolchain für vcpkg vor project() setzen
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

project(James)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")  # /MT für Release
add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE external)
target_compile_definitions(${PROJECT_NAME} PRIVATE JSON_NOEXCEPTION)

# PortAudio (static vcpkg)
set(PORTAUDIO_ROOT "$ENV{VCPKG_ROOT}/installed/x64-windows-static")
target_include_directories(${PROJECT_NAME} PRIVATE "${PORTAUDIO_ROOT}/include")
target_link_libraries(${PROJECT_NAME} PRIVATE "${PORTAUDIO_ROOT}/lib/portaudio.lib")

# Vosk
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/libs/vosk)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/libs/vosk/libvosk.lib)
