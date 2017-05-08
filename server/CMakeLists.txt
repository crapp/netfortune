#set(${PROJECT_NAME}_SERVER_HEADER)
set(${PROJECT_NAME}_SERVER_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    )

add_executable(netfortune-server
    "${${PROJECT_NAME}_SERVER_SOURCE}")

find_package(Boost REQUIRED COMPONENTS system)

message(WARNING "BIN DIR ${CMAKE_CURRENT_BINARY_DIR}")
message(WARNING "BOOST INC DIR ${Boost_INCLUDE_DIR}")

target_include_directories(netfortune-server
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
    PUBLIC ${Boost_INCLUDE_DIR}
)

target_link_libraries(netfortune-server ${Boost_LIBRARIES})

set_property(TARGET netfortune-server PROPERTY CXX_VERSION_REQUIRED ON)
set_property(TARGET netfortune-server PROPERTY CXX_VERSION 14)
