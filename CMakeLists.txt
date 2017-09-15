cmake_minimum_required(VERSION 3.5)

project(netfortune VERSION 0.1)

if(NOT FPROTO_MESSAGE_MAX)
    set(FPROTO_MESSAGE_MAX 2048)
endif()

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/config.hpp.in config_netfortune.hpp @ONLY)

add_subdirectory(src/fproto)
add_subdirectory(src/server)
add_subdirectory(src/client)

