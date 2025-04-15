cmake_minimum_required(VERSION 3.10)
project(PETSc4FOAM_by_lzr LANGUAGES CXX)

# Load custom modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(OpenFOAM REQUIRED)
find_package(PETSc REQUIRED)

# Set C++ standard (consistent with OpenFOAM)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable all warnings
add_compile_options(-Wall -Wextra -Wpedantic)

# Check consistency between declarations and definitions
add_compile_options(-Wmissing-declarations)  # Warn for functions defined without declarations
add_compile_options(-Wunused-function)       # Warn for unused static functions

# Add double-precision compile flag
add_definitions(-DWM_DP)

# Generate shared library
add_library(PETSc4FOAM_by_lzr SHARED
    src/Foam2PETSc.cpp
    src/KSPWrapper.cpp
    src/ParallelUtilities.cpp
)

# Include paths
target_include_directories(PETSc4FOAM_by_lzr PUBLIC
    ${OPENFOAM_INCLUDE_DIR}
    ${PETSC_INCLUDE_DIRS}
    include/
)

# Link libraries
target_link_libraries(PETSc4FOAM_by_lzr PUBLIC
    ${OPENFOAM_LIBRARIES}
    ${PETSC_LIBRARIES}
)

# Installation rules
install(TARGETS PETSc4FOAM_by_lzr DESTINATION lib)
install(DIRECTORY include/ DESTINATION include/PETSc4FOAM)
