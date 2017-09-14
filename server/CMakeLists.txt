set(${PROJECT_NAME}_SERVER_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/fserver.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fsession.hpp
    )
set(${PROJECT_NAME}_SERVER_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/fserver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fsession.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    )

add_executable(netfortune-server
    "${${PROJECT_NAME}_SERVER_SOURCE}" "${${PROJECT_NAME}_SERVER_HEADER}")

find_package(Boost REQUIRED COMPONENTS system)
message(WARNING "BIN DIR ${CMAKE_CURRENT_BINARY_DIR}")
message(WARNING "BOOST INC DIR ${Boost_INCLUDE_DIR}")
find_package(Threads)

target_include_directories(netfortune-server
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_BINARY_DIR}
    PUBLIC ${Boost_INCLUDE_DIR}
    PUBLIC "${CMAKE_SOURCE_DIR}/external"
    PUBLIC "${CMAKE_SOURCE_DIR}/external/json"
    PUBLIC "${CMAKE_SOURCE_DIR}/fproto"
    )

target_link_libraries(netfortune-server
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    fproto)

set_property(TARGET netfortune-server PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET netfortune-server PROPERTY CXX_STANDARD 14)
