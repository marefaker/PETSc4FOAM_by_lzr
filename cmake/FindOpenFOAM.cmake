# Locate the OpenFOAM header file path, specifically fvCFD.H
find_path(OPENFOAM_INCLUDE_DIR
    NAMES fvCFD.H
    PATHS /opt/openfoam12/src/OpenFOAM
    NO_DEFAULT_PATH
    DOC "Path to the OpenFOAM header file"
)

# Locate the OpenFOAM core library
find_library(OPENFOAM_CORE_LIB
    NAMES OpenFOAM
    PATHS /opt/openfoam12/platforms/linux64GccDPInt32Opt/lib
    DOC "Path to the OpenFOAM core library"
)

# Locate the finiteVolume module
find_library(OPENFOAM_FINITEVOLUME_LIB
    NAMES finiteVolume
    PATHS /opt/openfoam12/platforms/linux64GccDPInt32Opt/lib
    DOC "Path to the OpenFOAM finiteVolume library"
)

# Combine the libraries into a list
set(OPENFOAM_LIBRARIES
    ${OPENFOAM_CORE_LIB}
    ${OPENFOAM_FINITEVOLUME_LIB}
)

# Verify that the required components are found
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenFOAM
    REQUIRED_VARS 
        OPENFOAM_INCLUDE_DIR
        OPENFOAM_LIBRARIES
    FAIL_MESSAGE "Failed to locate the OpenFOAM installation. Please check the paths or run scripts/setup_dependencies.sh"
)
