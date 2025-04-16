# FindOpenFOAM.cmake for OpenFOAM.org v12+ (Modular Structure)
# Features:
# 1. Dynamic module detection
# 2. Platform-specific library resolution
# 3. Version validation
# 4. Component-based linking

# Basic path detection ---------------------------------------------------
if(NOT OPENFOAM_ROOT)
    set(OPENFOAM_ROOT "$ENV{WM_PROJECT_DIR}" CACHE PATH "OpenFOAM installation root")
endif()

if(NOT WM_OPTIONS)
    set(WM_OPTIONS "$ENV{WM_OPTIONS}" CACHE STRING "Build platform identifier")
endif()

# Version validation (must be >= v12) ------------------------------------
if(EXISTS "${OPENFOAM_ROOT}/etc/config.sh/version")
    file(STRINGS "${OPENFOAM_ROOT}/etc/config.sh/version" FOAM_VERSION_RAW 
         REGEX "FOAM_VERSION=.*")
    string(REGEX REPLACE "FOAM_VERSION=\"([0-9]+)\"" "\\1" OPENFOAM_VERSION ${FOAM_VERSION_RAW})
    if(OPENFOAM_VERSION LESS 12)
        message(FATAL_ERROR "Requires OpenFOAM >= v12 (found ${OPENFOAM_VERSION})")
    endif()
    message(STATUS "Found OpenFOAM v${OPENFOAM_VERSION}")
else()
    message(FATAL_ERROR "OpenFOAM version file missing")
endif()

# Modular include path generation ----------------------------------------
set(OPENFOAM_MODULES
    conversion
    fvMeshStitchers
    motionSolvers
    sampling
    MomentumTransportModels
    dummyThirdParty
    fvMeshTopoChangers
    multiphaseModels
    specieTransfer
    ODE
    fileFormats
    fvModels
    parallel
    surfMesh
    OSspecific
    finiteVolume
    fvMotionSolver
    physicalProperties
    thermophysicalModels
    OpenFOAM
    functionObjects
    generic
    polyTopoChange
    triSurface
    Pstream
    fvAgglomerationMethods
    lagrangian
    radiationModels
    twoPhaseModels
    ThermophysicalTransportModels
    fvConstraints
    mesh
    randomProcesses
    waves
    atmosphericModels
    fvMeshDistributors
    meshCheck
    renumber
    combustionModels
    fvMeshMovers
    meshTools
    rigidBodyMotion
    # All modules included in OpenFOAM v12 listed, delete as needed
)

set(OPENFOAM_INCLUDE_PATHS)
foreach(module IN LISTS OPENFOAM_MODULES)
    list(APPEND OPENFOAM_INCLUDE_PATHS
        "${OPENFOAM_ROOT}/src/${module}"             # Original headers
        "${OPENFOAM_ROOT}/src/${module}/lnInclude"   # Symlinked headers
    )
endforeach()
list(APPEND OPENFOAM_INCLUDE_PATHS
    "${OPENFOAM_ROOT}/platforms/${WM_OPTIONS}/include" # Platform-specific headers
)

# Key header file detection ----------------------------------------------
find_path(OPENFOAM_INCLUDE_DIR
    NAMES 
        OpenFOAM/primitiveFields.H   # Core type headers
        finiteVolume/fvCFD.H         # FVM headers
    PATHS ${OPENFOAM_INCLUDE_PATHS}
    NO_DEFAULT_PATH
    DOC "OpenFOAM modular include directories"
)

# Dynamic library location -----------------------------------------------
set(OPENFOAM_LIB_DIR "${OPENFOAM_ROOT}/platforms/${WM_OPTIONS}/lib")
set(REQUIRED_LIBS
    OpenFOAM          # Core library
    finiteVolume      # Finite volume library
    meshTools         # Mesh tools library
    # Add other libraries as needed...
)

set(OPENFOAM_LIBRARIES)
foreach(lib IN LISTS REQUIRED_LIBS)
    find_library(OPENFOAM_${lib}_LIB
        NAMES ${lib}
        PATHS ${OPENFOAM_LIB_DIR}
        NO_DEFAULT_PATH
        DOC "${lib} library path"
    )
    list(APPEND OPENFOAM_LIBRARIES ${OPENFOAM_${lib}_LIB})
endforeach()

# MPI support detection -------------------------------------------------
find_package(MPI REQUIRED)
list(APPEND OPENFOAM_LIBRARIES ${MPI_CXX_LIBRARIES})

# Final validation ------------------------------------------------------
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenFOAM
    REQUIRED_VARS 
        OPENFOAM_ROOT
        OPENFOAM_INCLUDE_DIR
        OPENFOAM_LIBRARIES
    VERSION_VAR OPENFOAM_VERSION
    FAIL_MESSAGE "
OpenFOAM configuration failed. Diagnostic info:
  WM_PROJECT_DIR: $ENV{WM_PROJECT_DIR}
  Detected paths:
    - Include dir: ${OPENFOAM_INCLUDE_DIR}
    - Library dir: ${OPENFOAM_LIB_DIR}
  Suggestions:
    1. Run 'source ${OPENFOAM_ROOT}/etc/bashrc'
    2. Verify 'platforms/${WM_OPTIONS}/lib' contains:
       ${REQUIRED_LIBS}
    3. Use -DOPENFOAM_ROOT=/path/to/openfoam"
)

# Target definition -----------------------------------------------------
if(OpenFOAM_FOUND AND NOT TARGET OpenFOAM::Core)
    add_library(OpenFOAM::Core INTERFACE IMPORTED)
    
    # Header configuration
    set_target_properties(OpenFOAM::Core PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${OPENFOAM_INCLUDE_DIR}"
        INTERFACE_COMPILE_DEFINITIONS "WM_ARCH_OPTION=${WM_OPTIONS}"
    )
    
    # Compiler options
    target_compile_options(OpenFOAM::Core INTERFACE
        -m64
        -std=c++14
        -fPIC
        -pthread
    )
    
    # Library linking
    target_link_libraries(OpenFOAM::Core INTERFACE
        ${OPENFOAM_LIBRARIES}
        dl
        m
        rt
    )
endif()