cmake_minimum_required(VERSION 3.15)  # Update CMake version for better compatibility
if(POLICY CMP0144)
    cmake_policy(SET CMP0144 NEW)
endif()
project(PETSc4FOAM_by_lzr
    LANGUAGES CXX
    VERSION 1.0.0
    DESCRIPTION "PETSc/OpenFOAM coupling library"
    HOMEPAGE_URL "https://github.com/marefaker/PETSc4FOAM_by_lzr"
)
set(CMAKE_CXX_STANDARD 14)
set(MPI_CXX_COMPILER /usr/bin/mpicxx)
set(MPI_C_COMPILER /usr/bin/mpicc)

# Debugging information
message(STATUS "Project Name: ${PROJECT_NAME}")
message(STATUS "Project Version: ${PROJECT_VERSION}")
message(STATUS "CMake Version: ${CMAKE_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")

set (CMAKE_CXX_FLAGS “${CMAKE_CXX_FLAGS} -g”)

find_package(MPI REQUIRED COMPONENTS CXX)
message(STATUS "MPI Found: ${MPI_FOUND}")
message(STATUS "MPI Include Path: ${MPI_CXX_INCLUDE_PATH}")
message(STATUS "MPI Libraries: ${MPI_CXX_LIBRARIES}")

#--- Module Configuration -------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
message(STATUS "CMake Module Path: ${CMAKE_MODULE_PATH}")

#--- OpenFOAM Configuration ------------------------------------------------------
find_package(OpenFOAM 11 REQUIRED
    COMPONENTS 
    atmosphericModels
    combustionModels
    conversion
    dummyThirdParty
    fileFormats
    finiteVolume
    functionObjects
    fvAgglomerationMethods
    fvConstraints
    fvMeshDistributors
    fvMeshMovers
    fvMeshStitchers
    fvMeshTopoChangers
    fvModels
    fvMotionSolver
    generic
    lagrangian
    mesh
    meshCheck
    meshTools
    momentumSolvers
    motionSolvers
    multiphaseModels
    ODE
    OpenFOAM
    parallel
    physicalProperties
    polyTopoChange
    psurface
    radiationModels
    randomProcesses
    renumber
    rigidBodyMotion
    sampling
    specieTransfer
    surfMesh
    ThermophysicalTransportModels
    thermophysicalModels
    triSurface
    twoPhaseModels
    waves
)
message(STATUS "OpenFOAM Found: ${OpenFOAM_FOUND}")
message(STATUS "OpenFOAM Include Path: ${OPENFOAM_INCLUDE_DIR}")
message(STATUS "OpenFOAM Library Path: ${OPENFOAM_LIBRARY_DIRS}")

#--- PETSc Configuration ---------------------------------------------------------
find_package(PETSc 3.23 REQUIRED 
    COMPONENTS 
        MPI
        VEC
        MAT
        KSP
)
message(STATUS "PETSc Found: ${PETSC_FOUND}")
message(STATUS "PETSc Include Path: ${PETSC_INCLUDE_DIRS}")
message(STATUS "PETSc Libraries: ${PETSC_LIBRARIES}")

#--- Compiler Setup --------------------------------------------------------------
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Architecture optimization
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-march=native" SUPPORT_NATIVE_ARCH)
if(SUPPORT_NATIVE_ARCH)
    add_compile_options(-march=native)
    message(STATUS "Native architecture optimization enabled.")
else()
    message(STATUS "Native architecture optimization not supported.")
endif()

# Warning configuration
add_compile_options(
    -Wall -Wextra -Wpedantic
)

# Debug symbols in both debug and release
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
add_definitions(-DWM_DP)

#--- Main Library Target ---------------------------------------------------------
add_library(PETSc4FOAM_by_lzr SHARED
    src/Foam2PETSc.cpp
    src/KSPWrapper.cpp
    src/ParallelUtilities.cpp
)
message(STATUS "Library Target Created: PETSc4FOAM_by_lzr")

#--- Include Directories ---------------------------------------------------------
target_include_directories(PETSc4FOAM_by_lzr PUBLIC
    # PETSc headers
    $<BUILD_INTERFACE:${PETSC_INCLUDE_DIRS}>
    $<INSTALL_INTERFACE:include>
    
    # OpenFOAM module headers
    ${OPENFOAM_INCLUDE_DIR}
    
    # Project headers
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# System directories for third-party includes
target_include_directories(PETSc4FOAM_by_lzr PRIVATE
    ${OPENFOAM_SYSTEM_INCLUDES}
)

#--- Link Configuration ----------------------------------------------------------
target_link_libraries(PETSc4FOAM_by_lzr PUBLIC
    # OpenFOAM core libraries
    ${OPENFOAM_INCLUDE_DIR}
    
    # Essential OpenFOAM modules
    finiteVolume
    meshTools
    fileFormats
    
    # PETSc libraries
    ${PETSC_LIBRARIES}
    
    # MPI support
    MPI::MPI_CXX
)
message(STATUS "Linking Libraries Configured for PETSc4FOAM_by_lzr")

#--- RPATH Management ------------------------------------------------------------
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH
    ${OPENFOAM_LIBRARY_DIRS}
    ${PETSC_LIBRARY_DIRS}
    $ORIGIN  # For relative path lookup
)
message(STATUS "RPATH Configuration: ${CMAKE_INSTALL_RPATH}")

#--- Installation Settings -------------------------------------------------------
install(TARGETS PETSc4FOAM_by_lzr 
    EXPORT PETSc4FOAM_by_lzr_Targets
    ARCHIVE  DESTINATION lib
    LIBRARY  DESTINATION lib
    RUNTIME  DESTINATION bin
)
message(STATUS "Installation Settings Configured")

install(DIRECTORY include/
    DESTINATION include/PETSc4FOAM
    PATTERN "*.h" 
    PATTERN "*.H"  # Include headers
    PATTERN "*.hpp"  # Header files in case of C++
)
message(STATUS "Header Files Installation Configured")

#--- Export Config ---------------------------------------------------------------
install(EXPORT PETSc4FOAM_by_lzr_Targets
    FILE PETSc4FOAM_by_lzrConfig.cmake
    DESTINATION lib/cmake/PETSc4FOAM_by_lzr
)
message(STATUS "Export Configuration Installed")

#--- Package Metadata ------------------------------------------------------------
include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/PETSc4FOAM_by_lzrConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/PETSc4FOAM_by_lzrConfig.cmake
    INSTALL_DESTINATION lib/cmake/PETSc4FOAM_by_lzr
)
message(STATUS "Package Metadata Configured")

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/PETSc4FOAMConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
message(STATUS "Package Version File Written")