# Define key variables
set(PETSC_DIR "$ENV{PETSC_DIR}" CACHE PATH "PETSc directory")
set(PETSC_ARCH "$ENV{PETSC_ARCH}" CACHE STRING "PETSc architecture")

# Locate PETSc header files
find_path(PETSC_INCLUDE_DIR
    NAMES petsc.h petscvec.h
    HINTS "${PETSC_DIR}/include" 
          "${PETSC_DIR}/${PETSC_ARCH}/include"
    DOC "Path to PETSc header files"
)

# Locate required libraries
find_library(PETSC_VEC_LIB
    NAMES petscvec
    HINTS "${PETSC_DIR}/${PETSC_ARCH}/lib"
    DOC "PETSc vector library"
)

find_library(PETSC_MAT_LIB
    NAMES petscmat
    HINTS "${PETSC_DIR}/${PETSC_ARCH}/lib"
    DOC "PETSc matrix library"
)

# Combine the libraries into a list
set(PETSC_LIBRARIES
    ${PETSC_VEC_LIB}
    ${PETSC_MAT_LIB}
    mpi  # Explicitly include MPI
)

# Verify that all required components are found
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PETSc
    REQUIRED_VARS PETSC_INCLUDE_DIR PETSC_LIBRARIES
    VERSION_VAR PETSC_VERSION  # Optional: handle version logic
)
