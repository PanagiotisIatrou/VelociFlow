cmake_minimum_required(VERSION 3.28)
project(Simulation)

set(CMAKE_CXX_STANDARD 17)

# Directory root for the simulation
set(SIMULATION_PATH SimulationLibrary)
set(EXAMPLES_PATH Examples)

# Include the library
include_directories(${SIMULATION_PATH})
add_subdirectory(${SIMULATION_PATH})

# Create the executable
add_executable(Simulation ${EXAMPLES_PATH}/simulation_continuation.cpp)

# Include the SimulationLibrary
target_link_libraries(Simulation SimulationLibrary)
