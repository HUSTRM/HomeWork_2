cmake_minimum_required(VERSION 3.12)
project(imageSegmentation)

set(CMAKE_CXX_STANDARD 14)

FIND_PACKAGE(OpenCV REQUIRED )

INCLUDE_DIRECTORIES( ${OPENCV_INCLUDE_DIR} )

ADD_EXECUTABLE(imageSegmentation main.cpp ThresholdMethods.cpp ThresholdMethods.h WatershedMethod.h WatershedMethod.cpp)
TARGET_LINK_LIBRARIES(imageSegmentation ${OpenCV_LIBS})