# Adapted FindOpenFOAM.cmake for OpenFOAM.org v12+ modular header structure

# Use environment variables or default paths
if(NOT DEFINED OPENFOAM_ROOT)
    set(OPENFOAM_ROOT "$ENV{WM_PROJECT_DIR}" CACHE PATH "OpenFOAM installation root directory")
endif()
if(NOT DEFINED WM_OPTIONS)
    set(WM_OPTIONS "$ENV{WM_OPTIONS}" CACHE STRING "OpenFOAM platform configuration (e.g. linux64GccDPInt32Opt)")
endif()

# Core header file paths (modular structure)
set(OPENFOAM_HEADER_PATHS
    "${OPENFOAM_ROOT}/src/OpenFOAM"
    "${OPENFOAM_ROOT}/src/finiteVolume"
    "${OPENFOAM_ROOT}/src/meshTools"
    "${OPENFOAM_ROOT}/platforms/${WM_OPTIONS}/include"
)

# Verify OpenFOAM version (v12+)
if(EXISTS "${OPENFOAM_ROOT}/etc/config.sh/version")
    file(STRINGS "${OPENFOAM_ROOT}/etc/config.sh/version" FOAM_VERSION_RAW
         REGEX "FOAM_VERSION=.*")
    string(REGEX REPLACE "FOAM_VERSION=\"([0-9]+)\"" "\\1" OPENFOAM_VERSION ${FOAM_VERSION_RAW})
    if(OPENFOAM_VERSION LESS 12)
        message(FATAL_ERROR "Requires OpenFOAM >= v12 (found ${OPENFOAM_VERSION})")
    endif()
endif()

# Locate key header files (replace fvCFD.H)
find_path(OPENFOAM_INCLUDE_DIR
    NAMES 
        OpenFOAM/OpenFOAM.H          # Core header file
        finiteVolume/fvCFD.H          # Backward compatibility (if exists)
    PATHS ${OPENFOAM_HEADER_PATHS}
    NO_DEFAULT_PATH
    DOC "Path to OpenFOAM modular header files"
)

# Locate core library
set(OPENFOAM_LIB_PATHS
    "${OPENFOAM_ROOT}/platforms/${WM_OPTIONS}/lib"
    "${OPENFOAM_ROOT}/platforms/${WM_OPTIONS}/lib/dummy" # Prevent empty path errors
)

find_library(OPENFOAM_CORE_LIB
    NAMES OpenFOAM
    PATHS ${OPENFOAM_LIB_PATHS}
    NO_DEFAULT_PATH
    DOC "OpenFOAM core library"
)

# Locate key module libraries
set(REQUIRED_OPENFOAM_LIBS
    finiteVolume
    meshTools
    fvOptions
)

foreach(lib ${REQUIRED_OPENFOAM_LIBS})
    find_library(OPENFOAM_${lib}_LIB
        NAMES ${lib}
        PATHS ${OPENFOAM_LIB_PATHS}
        NO_DEFAULT_PATH
        DOC "${lib} module library"
    )
    list(APPEND OPENFOAM_LIBRARIES ${OPENFOAM_${lib}_LIB})
endforeach()

# Combine library list
set(OPENFOAM_LIBRARIES
    ${OPENFOAM_CORE_LIB}
    ${OPENFOAM_LIBRARIES}
    CACHE STRING "Collection of OpenFOAM libraries"
)

# Final verification
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenFOAM
    REQUIRED_VARS 
        OPENFOAM_INCLUDE_DIR
        OPENFOAM_CORE_LIB
        OPENFOAM_LIBRARIES
    VERSION_VAR OPENFOAM_VERSION
    FAIL_MESSAGE "OpenFOAM configuration failed. Please check: (1) WM_PROJECT_DIR environment variable points to the correct path, (2) platforms/${WM_OPTIONS}/lib contains the required libraries, (3) specify the correct installation path using -DOPENFOAM_ROOT."
)
