add_library(graph graph.cpp)
target_include_directories(graph PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(data_structures main.cpp graph.cpp)
