set(${PROJECT_NAME}_fproto_header
    ${CMAKE_CURRENT_SOURCE_DIR}/fproto.hpp
    )

set(${PROJECT_NAME}_fproto_src
    ${CMAKE_CURRENT_SOURCE_DIR}/fproto.cpp
    )


add_library(fproto ${${PROJECT_NAME}_fproto_src} ${${PROJECT_NAME}_fproto_src})

target_include_directories(fproto
    PUBLIC ${CMAKE_SOURCE_DIR}/external
    PUBLIC ${CMAKE_SOURCE_DIR}/external/json
    PUBLIC ${CMAKE_BINARY_DIR}
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
    )

set_property(TARGET fproto PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET fproto PROPERTY CXX_STANDARD 14)
