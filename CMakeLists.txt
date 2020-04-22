cmake_minimum_required(VERSION 3.5)

project(Aura LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenCV_DIR "/usr/local/Cellar/opencv/4.2.0_3/lib/cmake/opencv4")
set(AVCODEC_INCLUDE_DIR "/usr/local/Cellar/ffmpeg/4.2.2_2/lib")
set(LIBSND_INCLUDE_DIR "/usr/local/Cellar/libsndfile/1.0.28/lib")
set(LIB_PORTAUDIO_DIR "/usr/local/Cellar/portaudio/19.6.0/lib")
find_library(AVCODEC_LIBRARY avcodec)
find_library(AVFORMAT_LIBRARY avformat)
find_library(AVUTIL_LIBRARY avutil)
find_library(AVDEVICE_LIBRARY avdevice)
find_library(AVFILTER_LIBRARY avfilter)
find_library(FFTW3_LIBRARY fftw3)
find_library(LIBSNDFILE_LIBRARY SndFile)
find_library(LIB_PORTAUDIO portaudio)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} ${LIB_PORTAUDIO_DIR})
find_package(Qt5 COMPONENTS Core Quick Multimedia Widgets Charts REQUIRED)

find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
find_path(AVDEVICE_INCLUDE_DIR libavdevice/avdevice.h)

add_executable(Aura
    main.cpp
    video_provider.cpp
    video_worker_thread.cpp
#    image_provider.cpp
    image_worker_thread.cpp
    image_item.cpp
    audio_provider.cpp
    audio_worker.cpp
    qml.qrc
    )

target_include_directories(Aura PRIVATE ${AVCODEC_INCLUDE_DIR} ${AVFORMAT_INCLUDE_DIR} ${AVUTIL_INCLUDE_DIR} ${AVDEVICE_INCLUDE_DIR} ${LIBSND_INCLUDE_DIR} ${LIB_PORTAUDIO_DIR})
target_compile_definitions(Aura
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(Aura
    PRIVATE Qt5::Core Qt5::Quick Qt5::Multimedia Qt5::Widgets Qt5::Charts ${OpenCV_LIBS} ${AVCODEC_LIBRARY} ${AVFORMAT_LIBRARY} ${AVUTIL_LIBRARY} ${AVDEVICE_LIBRARY} ${AVFILTER_LIBRARY} ${FFTW3_LIBRARY} ${LIBSNDFILE_LIBRARY} ${LIB_PORTAUDIO})
