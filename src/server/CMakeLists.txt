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
    HINTS "/usr/include" "/usr/local/include" "/opt/local/include"
)
find_library(
    SQLITE_LIB
    sqlite3
    HINTS "/usr/lib" "/usr/local/lib" "/opt/local/lib"
)
if(NOT SQLITE_HEADER)
    message(SEND_ERROR "Failed to find sqlite3 header")
endif()
if(NOT SQLITE_LIB)
    message(SEND_ERROR "Failed to find sqlite3 library")
endif()

add_executable(netfortune-server
    "${${PROJECT_NAME}_SERVER_SOURCE}" "${${PROJECT_NAME}_SERVER_HEADER}")

find_package(Boost REQUIRED COMPONENTS system filesystem)
message(WARNING "BIN DIR ${CMAKE_CURRENT_BINARY_DIR}")
message(WARNING "BOOST INC DIR ${Boost_INCLUDE_DIR}")
find_package(Threads)

target_include_directories(netfortune-server
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_BINARY_DIR}
    PUBLIC ${Boost_INCLUDE_DIR}
    PUBLIC ${SQLITE_HEADER}
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

set_property(TARGET netfortune-server PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET netfortune-server PROPERTY CXX_STANDARD 14)
