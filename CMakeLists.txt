cmake_minimum_required(VERSION 3.5)

project(Aura LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(OpenCV REQUIRED)
find_package(Portaudio REQUIRED)
find_package(SndFile REQUIRED)
find_package(Qt5 COMPONENTS Core Quick Multimedia Widgets Charts REQUIRED)

#find_library(FFTW3_LIBRARY fftw3)
#find_library(LIBSNDFILE_LIBRARY SndFile)
#find_library(PORTAUDIO_LIBRARY portaudio)

message(STATUS "OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")

add_executable(Aura
    main.cpp
    video_provider.cpp
    video_worker_thread.cpp
    image_worker_thread.cpp
    image_item.cpp
    audio_provider.cpp
    audio_worker.cpp
    qml.qrc
    )

target_include_directories(Aura PRIVATE ${PORTAUDIO_INCLUDE_DIRS} ${LIBSND_DIR})
target_compile_definitions(Aura
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(Aura
    PRIVATE Qt5::Core Qt5::Quick Qt5::Multimedia Qt5::Widgets Qt5::Charts  ${OpenCV_LIBS} ${PORTAUDIO_LIBRARY} SndFile::sndfile)
