add_library(jspp INTERFACE)

target_include_directories(jspp INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(jspp INTERFACE js)