cmake_minimum_required(VERSION 2.8)
project(mavkit)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

################################################################################
#GENERATE MAVLINK HEADERS
set(MAVLINK_DIR ../include/mavlink)
set(MAVLINK_VERSION 2.0)
set(MAVLINK_HEADERS_DIR generated/mavlink_headers)
execute_process(COMMAND python ${MAVLINK_DIR}/pymavlink/tools/mavgen.py --lang=C --wire-protocol=${MAVLINK_VERSION} --output=${MAVLINK_HEADERS_DIR} ${MAVLINK_DIR}/message_definitions/v1.0/ardupilotmega.xml)

################################################################################
#PACKAGES
find_package(Boost REQUIRED COMPONENTS regex)
find_package(Threads REQUIRED)

################################################################################
#INCLUDES
include_directories(
    include
    ${CMAKE_BINARY_DIR}/generated
    ${Boost_INCLUDE_DIRS}
)

################################################################################
#SOURCES
file(GLOB_RECURSE MAVKIT_SRC "src/mavkit/*.cpp")

################################################################################
#LINK
LIST(APPEND LINK_LIBS
	${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

################################################################################
#EXECUTABLE
add_executable(${PROJECT_NAME} ${MAVKIT_SRC} src/mavkit.cpp)
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})
