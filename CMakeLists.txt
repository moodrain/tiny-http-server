cmake_minimum_required(VERSION 3.21)
project(tiny-http-server)

set(TARGET_BIN "tiny-http-server")

include(CheckIncludeFiles)
include_directories(. /usr/local/include)

aux_source_directory(./src DIR_SRCS)
add_subdirectory(libs/)

find_package(uriparser)

add_executable(${TARGET_BIN} ${DIR_SRCS})
target_link_libraries(${TARGET_BIN} PUBLIC core pthread uriparser::uriparser)