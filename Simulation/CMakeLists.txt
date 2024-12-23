cmake_minimum_required(VERSION 3.28)
project(Simulation)

set(CMAKE_CXX_STANDARD 17)

# Directory root for the simulation
set(SIMULATION_PATH src)

# Directory root for the tests
set(TESTS_PATH tests)

file(GLOB_RECURSE SIMULATION_FILES
        ${SIMULATION_PATH}/*.cpp
        ${SIMULATION_PATH}/*.hpp
)

add_executable(Simulation ${SIMULATION_FILES})
