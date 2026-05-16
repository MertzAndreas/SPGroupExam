cmake_minimum_required(VERSION 3.20)

project(stochastic_sim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)
find_package(Catch2 3 REQUIRED)


add_library(stochastic
    src/reaction.cpp
    src/simulator.cpp
    src/vessel.cpp
    src/graph_visitor.cpp
    src/chart.cpp
)

target_include_directories(stochastic
    PUBLIC include
)

target_link_libraries(stochastic
    PUBLIC Qt6::Widgets Qt6::Charts
)

add_executable(decay examples/decay.cpp)
target_link_libraries(decay PRIVATE stochastic)

add_executable(covid examples/covid.cpp)
target_link_libraries(covid PRIVATE stochastic)

add_executable(circadian examples/circadian.cpp)
target_link_libraries(circadian PRIVATE stochastic)

# Test 
enable_testing()

add_executable(test_graph_visitor
    tests/test_graph_visitor.cpp
)

target_link_libraries(test_graph_visitor
  PRIVATE stochastic Catch2::Catch2WithMain
)

add_executable(test_symbol_table
    tests/test_symbol_table.cpp
)

target_link_libraries(test_symbol_table
  PRIVATE stochastic Catch2::Catch2WithMain
)

add_test(NAME reactions COMMAND test_reactions)
add_test(NAME symbol_table COMMAND test_symbol_table)
