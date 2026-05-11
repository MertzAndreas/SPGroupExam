cmake_minimum_required(VERSION 3.20)

project(stochastic_sim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(stochastic
    src/reaction.cpp
    src/simulator.cpp
    src/vessel.cpp
    src/graph_visitor.cpp
)

target_include_directories(stochastic
    PUBLIC include
)

add_executable(decay examples/decay.cpp)
target_link_libraries(decay PRIVATE stochastic)

add_executable(covid examples/covid.cpp)
target_link_libraries(covid PRIVATE stochastic)

add_executable(circadian examples/circadian.cpp)
target_link_libraries(circadian PRIVATE stochastic)

# -------------------
# Tests
# -------------------

enable_testing()

add_executable(test_reactions
    tests/test_reactions.cpp
)

target_link_libraries(test_reactions
    PRIVATE stochastic
)

add_executable(test_symbol_table
    tests/test_symbol_table.cpp
)

target_link_libraries(test_symbol_table
    PRIVATE stochastic
)

add_test(NAME reactions COMMAND test_reactions)
add_test(NAME symbol_table COMMAND test_symbol_table)
