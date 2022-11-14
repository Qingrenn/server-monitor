cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(svr-monitor LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Release)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(workflow_DIR /home/qingren/Project/workflow)
set(wfrest_DIR /home/qingren/Project/wfrest)
find_package(wfrest REQUIRED CONFIG)
find_package(workflow REQUIRED CONFIG)

include_directories(
  ${WFREST_INCLUDE_DIR}
  ${WORKFLOW_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/app
)
link_directories(${WFREST_LIB_DIR} ${WORKFLOW_LIB_DIR})

add_subdirectory(app)

add_executable(server main.cpp)
target_link_libraries(server PRIVATE app wfrest workflow)
