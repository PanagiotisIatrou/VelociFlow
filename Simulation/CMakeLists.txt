cmake_minimum_required(VERSION 3.28)
project(Simulation)

set(CMAKE_CXX_STANDARD 17)

# Directory root for the simulation
set(SIMULATION_PATH src)

# Directory root for the tests
set(TESTS_PATH tests)

add_executable(Simulation
        ${SIMULATION_PATH}/main.cpp
        ${SIMULATION_PATH}/MeshExamples.hpp

        ${SIMULATION_PATH}/Simulation/common.hpp
        ${SIMULATION_PATH}/Simulation/SteadySimulation.cpp
        ${SIMULATION_PATH}/Simulation/SteadySimulation.hpp
        ${SIMULATION_PATH}/Simulation/UnsteadySimulation.cpp
        ${SIMULATION_PATH}/Simulation/UnsteadySimulation.hpp

        ${SIMULATION_PATH}/Simulation/Simulation/Simulation.cpp
        ${SIMULATION_PATH}/Simulation/Simulation/Simulation.hpp
        ${SIMULATION_PATH}/Simulation/Simulation/BulkNodeOperations.cpp
        ${SIMULATION_PATH}/Simulation/Simulation/BulkNodeOperations.hpp
        ${SIMULATION_PATH}/Simulation/Simulation/BulkFaceOperations.cpp
        ${SIMULATION_PATH}/Simulation/Simulation/BulkFaceOperations.hpp

        ${SIMULATION_PATH}/Simulation/Simulation/Utilities/Timer.cpp
        ${SIMULATION_PATH}/Simulation/Simulation/Utilities/Timer.hpp
        ${SIMULATION_PATH}/Simulation/Simulation/Utilities/Saver.cpp
        ${SIMULATION_PATH}/Simulation/Simulation/Utilities/Saver.hpp

        ${SIMULATION_PATH}/Simulation/Simulation/Meshing/Mesh.cpp
        ${SIMULATION_PATH}/Simulation/Simulation/Meshing/Mesh.hpp

        ${SIMULATION_PATH}/Simulation/Simulation/Meshing/Nodes/Node.hpp
        ${SIMULATION_PATH}/Simulation/Simulation/Meshing/Nodes/Node.cpp
        ${SIMULATION_PATH}/Simulation/Simulation/Meshing/Nodes/Coefficients/Coefficients.cpp
        ${SIMULATION_PATH}/Simulation/Simulation/Meshing/Nodes/Coefficients/Coefficients.hpp
        ${SIMULATION_PATH}/Simulation/Simulation/Meshing/Nodes/Coefficients/PressureCoefficients/PressureCoefficients.hpp
        ${SIMULATION_PATH}/Simulation/Simulation/Meshing/Nodes/Coefficients/PressureCoefficients/PressureCoefficients.cpp
        ${SIMULATION_PATH}/Simulation/Simulation/Meshing/Nodes/Coefficients/MomentumCoefficients/MomentumCoefficients.hpp
        ${SIMULATION_PATH}/Simulation/Simulation/Meshing/Nodes/Coefficients/MomentumCoefficients/MomentumCoefficients.cpp

        ${SIMULATION_PATH}/Simulation/Simulation/Meshing/Faces/Face.cpp
        ${SIMULATION_PATH}/Simulation/Simulation/Meshing/Faces/Face.hpp
        ${SIMULATION_PATH}/Simulation/Simulation/Meshing/Faces/Boundary/BoundaryFace.cpp
        ${SIMULATION_PATH}/Simulation/Simulation/Meshing/Faces/Boundary/BoundaryFace.hpp
        ${SIMULATION_PATH}/Simulation/Simulation/Meshing/Faces/Interior/InteriorFace.cpp
        ${SIMULATION_PATH}/Simulation/Simulation/Meshing/Faces/Interior/InteriorFace.hpp
        ${SIMULATION_PATH}/Simulation/Simulation/Meshing/Faces/Interior/InteriorFaceX.hpp
        ${SIMULATION_PATH}/Simulation/Simulation/Meshing/Faces/Interior/InteriorFaceX.cpp
        ${SIMULATION_PATH}/Simulation/Simulation/Meshing/Faces/Interior/InteriorFaceY.hpp
        ${SIMULATION_PATH}/Simulation/Simulation/Meshing/Faces/Interior/InteriorFaceY.cpp
        ${SIMULATION_PATH}/Simulation/Simulation/Meshing/Faces/Boundary/FixedVelocityBoundaryFace.cpp
        ${SIMULATION_PATH}/Simulation/Simulation/Meshing/Faces/Boundary/FixedVelocityBoundaryFace.hpp
        ${SIMULATION_PATH}/Simulation/Simulation/Meshing/Faces/Boundary/FixedPressureBoundaryFace.cpp
        ${SIMULATION_PATH}/Simulation/Simulation/Meshing/Faces/Boundary/FixedPressureBoundaryFace.hpp
        ${SIMULATION_PATH}/Simulation/Simulation/Meshing/Faces/Boundary/FreeBoundaryFace.cpp
        ${SIMULATION_PATH}/Simulation/Simulation/Meshing/Faces/Boundary/FreeBoundaryFace.hpp

        ${TESTS_PATH}/tests.hpp
        ${TESTS_PATH}/helper.hpp
)
