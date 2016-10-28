cmake_minimum_required(VERSION 2.8)
project(mavkit)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

################################################################################
#PACKAGES
find_package(Threads REQUIRED)

################################################################################
#INCLUDES
include_directories(
    include
)

################################################################################
#SOURCES
file(GLOB_RECURSE MAVKIT_SRC "src/mavkit/*.cpp")

################################################################################
#LINK
LIST(APPEND LINK_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
)

################################################################################
#EXECUTABLE
add_executable(${PROJECT_NAME} ${MAVKIT_SRC} src/mavkit.cpp)
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})
