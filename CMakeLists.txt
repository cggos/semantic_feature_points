cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(SemanticFeaturePoints)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

message("Build type: " ${CMAKE_BUILD_TYPE})

# set one ON
option(BUILD_SP  "build SuperPoint feature points" ON)
option(BUILD_GCN "build GCNv2 feature points" OFF)

add_definitions(-w)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -Wall  -O3 -march=native -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall   -O3 -march=native -g")

find_package(OpenCV 3.0)
if(NOT OpenCV_FOUND)
  include_directories(${OpenCV_INCLUDE_DIRS})
endif()

if(BUILD_SP)
  message("BUILD_SP: " ${BUILD_SP})
  set(CMAKE_PREFIX_PATH "/home/cg/projects/3rdparty/libtorch-1.2.0/share/cmake/Torch")
endif()
if(BUILD_GCN)
  message("BUILD_GCN: " ${BUILD_GCN})
  # use the latest
  set(CMAKE_PREFIX_PATH "/home/cg/projects/3rdparty/libtorch/share/cmake/Torch")
endif()
# set(Torch_DIR "/home/cg/projects/3rdparty/pytorch/torch/lib/tmp_install/share/cmake/Torch/")
find_package(Torch REQUIRED)
if(Torch_FOUND)
  message(================== ${TORCH_LIBRARIES}) 
  add_definitions(-DTORCH_NEW_API)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")
endif()

include_directories(include)

if(BUILD_SP)
  add_definitions(-DBUILD_SP)
  add_executable(super_point src/main.cc src/SuperPoint.cc src/SPextractor.cc src/GCNextractor.cc)
  target_link_libraries(super_point ${OpenCV_LIBS} ${TORCH_LIBRARIES})
  set_property(TARGET super_point PROPERTY CXX_STANDARD 14)
endif()

if(BUILD_GCN)
  add_definitions(-DBUILD_GCN)
  add_executable(gcn_ftr src/main.cc src/GCNextractor.cc)
  target_link_libraries(gcn_ftr ${OpenCV_LIBS} ${TORCH_LIBRARIES})
  set_property(TARGET gcn_ftr PROPERTY CXX_STANDARD 14)
endif()