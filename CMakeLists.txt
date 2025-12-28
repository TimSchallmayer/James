cmake_minimum_required(VERSION 3.20)

# Toolchain für vcpkg vor project() setzen
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

project(James)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")  # /MT für Release
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
file(GLOB GGML_SOURCES "libs/whisper.cpp/ggml/src/*.cpp" "libs/whisper.cpp/ggml/src/*.c")
add_executable(James main.cpp libs/whisper.cpp/src/whisper.cpp ${GGML_SOURCES})


#whisper
add_definitions(-DWHISPER_VERSION=\"1.0\")
target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/libs/whisper.cpp/include"       # whisper.h
    "${CMAKE_SOURCE_DIR}/libs/whisper.cpp/ggml/include"  # ggml.h
)

# nlohmann_json
target_include_directories(${PROJECT_NAME} PRIVATE external)
target_compile_definitions(${PROJECT_NAME} PRIVATE JSON_NOEXCEPTION)

# PortAudio (static vcpkg)
set(PORTAUDIO_ROOT "$ENV{VCPKG_ROOT}/installed/x64-windows-static")
target_include_directories(${PROJECT_NAME} PRIVATE "${PORTAUDIO_ROOT}/include")
target_link_libraries(${PROJECT_NAME} PRIVATE "${PORTAUDIO_ROOT}/lib/portaudio.lib")

# Vosk
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/libs/vosk)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/libs/vosk/libvosk.lib)
