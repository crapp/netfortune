# include this module to check for needed sqlite3 symbols
include(CheckSymbolExists)

set(${PROJECT_NAME}_SERVER_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/configuration.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dbcon.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fserver.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fsession.hpp
    )
set(${PROJECT_NAME}_SERVER_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/dbcon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fserver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fsession.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    )

#find sqlite3 header and library
find_path(
    SQLITE_HEADER
    sqlite3.h
    HINTS "/opt/local/include"
    NO_DEFAULT_PATH
)
find_path(
    SQLITE_HEADER
    sqlite3.h
)
find_library(
    SQLITE_LIB
    sqlite3
    HINTS "/opt/local/lib"
    NO_DEFAULT_PATH
)
find_library(
    SQLITE_LIB
    sqlite3
)
message(STATUS "SQLITE3_HEADER: [${SQLITE_HEADER}]")
message(STATUS "SQLITE3_LIB: [${SQLITE_LIB}]")

if(NOT SQLITE_HEADER)
    message(SEND_ERROR "Failed to locate sqlite3 header")
endif()
if(NOT SQLITE_LIB)
    message(SEND_ERROR "Failed to locate sqlite3 library")
endif()

add_executable(netfortune-server
    "${${PROJECT_NAME}_SERVER_SOURCE}" "${${PROJECT_NAME}_SERVER_HEADER}")

find_package(Boost REQUIRED COMPONENTS system filesystem)
message(WARNING "BIN DIR ${CMAKE_CURRENT_BINARY_DIR}")
message(WARNING "BOOST INC DIR ${Boost_INCLUDE_DIR}")
find_package(Threads)

target_include_directories(netfortune-server SYSTEM
    PRIVATE ${SQLITE_HEADER}
    PRIVATE ${Boost_INCLUDE_DIR}
    )

target_include_directories(netfortune-server
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_BINARY_DIR}
    PUBLIC "${CMAKE_SOURCE_DIR}/external"
    PUBLIC "${CMAKE_SOURCE_DIR}/external/json"
    PUBLIC "${CMAKE_SOURCE_DIR}/external/sqlite_modern_cpp/hdr"
    PUBLIC "${CMAKE_SOURCE_DIR}/src/fproto"
    )

target_link_libraries(netfortune-server
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    ${SQLITE_LIB}
    fproto)

#check_symbol_exists("sizeof" "stdio.h" HAVE_SQLEXP)
#message(STATUS "HAVE_SQLEXP: ${HAVE_SQLEXP}")

set_property(TARGET netfortune-server PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET netfortune-server PROPERTY CXX_STANDARD 14)
