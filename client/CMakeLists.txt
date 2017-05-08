#set(${PROJECT_NAME}_SERVER_HEADER)
set(${PROJECT_NAME}_CLIENT_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    )

add_executable(netfortune-client ${${PROJECT_NAME}_CLIENT_SOURCE})


set_property(TARGET netfortune-client PROPERTY CXX_VERSION_REQUIRED ON)
set_property(TARGET netfortune-client PROPERTY CXX_VERSION 14)
