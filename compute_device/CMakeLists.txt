cmake_minimum_required(VERSION 3.1)

set(CMAKE_CUDA_ARCHITECTURES 60 61 62 70 72 75 86 89 90)
set(CMAKE_CUDA_COMPILER /usr/local/cuda/bin/nvcc)

project(compute_device LANGUAGES CXX CUDA)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)
option(CUDA_USE_STATIC_CUDA_RUNTIME OFF)

# CUDA
find_package(CUDA REQUIRED)
message(STATUS "CUDA Libs: \n${CUDA_LIBRARIES}\n")
get_filename_component(CUDA_LIB_DIR ${CUDA_LIBRARIES} DIRECTORY)
message(STATUS "CUDA Headers: \n${CUDA_INCLUDE_DIRS}\n")

# OpenCV
find_package(OpenCV REQUIRED)
message(STATUS "OpenCV Libs: \n${OpenCV_LIBS}\n")
message(STATUS "OpenCV Libraries: \n${OpenCV_LIBRARIES}\n")
message(STATUS "OpenCV Headers: \n${OpenCV_INCLUDE_DIRS}\n")

# TensorRT
# TENSOR_HOME=/home/nhwn/yolo/TensorRT-8.6.1.6
set(TensorRT_INCLUDE_DIRS "$ENV{TENSOR_HOME}/include")
set(TensorRT_LIBRARIES "$ENV{TENSOR_HOME}/lib")
# set(TensorRT_INCLUDE_DIRS /usr/include/x86_64-linux-gnu)
# set(TensorRT_LIBRARIES /usr/lib/x86_64-linux-gnu)


message(STATUS "TensorRT Libs: \n${TensorRT_LIBRARIES}\n")
message(STATUS "TensorRT Headers: \n${TensorRT_INCLUDE_DIRS}\n")

list(APPEND INCLUDE_DIRS
        ${CUDA_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
        ${TensorRT_INCLUDE_DIRS}
        include
        )

list(APPEND ALL_LIBS
        ${CUDA_LIBRARIES}
        ${CUDA_LIB_DIR}
        ${OpenCV_LIBRARIES}
        ${TensorRT_LIBRARIES}
        )

include_directories(${INCLUDE_DIRS})

add_executable(${PROJECT_NAME}
        common.cpp
        main.cpp
        player.cpp
        strategy.cpp
        dealer.cpp
        matrix.cpp
        shoe.cpp
        include/common.h
        include/matrix.h
        include/shoe.h
        include/common.hpp
        include/params.h
        include/strategy.h
        include/dealer.h
        include/player.h
        include/game.h
        include/yolov8.hpp
        include/bridge.h
        include/timer.h
        include/config.h
)

target_link_directories(${PROJECT_NAME} PUBLIC ${ALL_LIBS})
target_link_libraries(${PROJECT_NAME} PRIVATE nvinfer nvinfer_plugin cudart ${OpenCV_LIBS})
