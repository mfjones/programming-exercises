add_library(graph graph.cpp)
target_include_directories(graph PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(data_structures_xc main.cpp graph.cpp)
