cmake_minimum_required(VERSION 3.15)  # Update CMake version for better compatibility
project(PETSc4FOAM_by_lzr
    LANGUAGES CXX
    DESCRIPTION "PETSc/OpenFOAM coupling library"
    HOMEPAGE_URL "https://github.com/yourusername/PETSc4FOAM"
)

#--- Module Configuration -------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

#--- OpenFOAM Configuration ------------------------------------------------------
find_package(OpenFOAM 12 REQUIRED
    COMPONENTS 
        OpenFOAM          # Core data structures
        finiteVolume      # FV discretization
        meshTools         # Mesh operations
        fileFormats       # File I/O support
)

#--- PETSc Configuration ---------------------------------------------------------
find_package(PETSc 3.23 REQUIRED 
    COMPONENTS 
        MPI
        VEC
        MAT
        KSP
)

#--- Compiler Setup --------------------------------------------------------------
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Architecture optimization
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-march=native" SUPPORT_NATIVE_ARCH)
if(SUPPORT_NATIVE_ARCH)
    add_compile_options(-march=native)
endif()

# Warning configuration
add_compile_options(
    -Wall -Wextra -Wpedantic
    -Wno-unused-parameter -Wno-old-style-cast
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

#--- Include Directories ---------------------------------------------------------
target_include_directories(PETSc4FOAM_by_lzr PUBLIC
    # PETSc headers
    $<BUILD_INTERFACE:${PETSC_INCLUDE_DIRS}>
    $<INSTALL_INTERFACE:include>
    
    # OpenFOAM module headers
    ${OPENFOAM_INCLUDE_DIRS}
    
    # Project headers
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# System directories for third-party includes
target_include_system_directories(PETSc4FOAM_by_lzr PRIVATE
    ${OPENFOAM_SYSTEM_INCLUDES}
)

#--- Link Configuration ----------------------------------------------------------
target_link_libraries(PETSc4FOAM_by_lzr PUBLIC
    # OpenFOAM core libraries
    ${OPENFOAM_CORE_LIBRARIES}
    
    # Essential OpenFOAM modules
    OpenFOAM::finiteVolume
    OpenFOAM::meshTools
    OpenFOAM::fileFormats
    
    # PETSc libraries
    ${PETSC_LIBRARIES}
    
    # MPI support
    MPI::MPI_CXX
)

#--- RPATH Management ------------------------------------------------------------
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH
    ${OPENFOAM_LIB_DIR}
    ${PETSC_LIBRARY_DIRS}
    $ORIGIN  # For relative path lookup
)

#--- Installation Settings -------------------------------------------------------
install(TARGETS PETSc4FOAM_by_lzr
    ARCHIVE  DESTINATION lib
    LIBRARY  DESTINATION lib
    RUNTIME  DESTINATION bin
)

install(DIRECTORY include/
    DESTINATION include/PETSc4FOAM
    PATTERN "*.hpp"
)

#--- Export Config ---------------------------------------------------------------
install(EXPORT PETSc4FOAM_Targets
    FILE PETSc4FOAMConfig.cmake
    DESTINATION lib/cmake/PETSc4FOAM
)

#--- Package Metadata ------------------------------------------------------------
include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/PETSc4FOAMConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/PETSc4FOAMConfig.cmake
    INSTALL_DESTINATION lib/cmake/PETSc4FOAM
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/PETSc4FOAMConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)