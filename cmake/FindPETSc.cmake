# 定义关键变量
set(PETSC_DIR "$ENV{PETSC_DIR}" CACHE PATH "PETSc根目录")
set(PETSC_ARCH "$ENV{PETSC_ARCH}" CACHE STRING "PETSc架构名称")

# 定位PETSc头文件
find_path(PETSC_INCLUDE_DIR
    NAMES petsc.h petscvec.h
    HINTS "${PETSC_DIR}/include" 
          "${PETSC_DIR}/${PETSC_ARCH}/include"
    DOC "PETSc头文件路径"
)

# 定位关键库
find_library(PETSC_VEC_LIB
    NAMES petscvec
    HINTS "${PETSC_DIR}/${PETSC_ARCH}/lib"
    DOC "PETSc矢量库"
)

find_library(PETSC_MAT_LIB
    NAMES petscmat
    HINTS "${PETSC_DIR}/${PETSC_ARCH}/lib"
    DOC "PETSc矩阵库"
)

# 合并库文件列表
set(PETSC_LIBRARIES
    ${PETSC_VEC_LIB}
    ${PETSC_MAT_LIB}
    mpi  # 需要显式链接MPI库
)

# 验证是否完成配置
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PETSc
    REQUIRED_VARS PETSC_INCLUDE_DIR PETSC_LIBRARIES
    VERSION_VAR PETSC_VERSION  # 需要版本检测逻辑（可选）
)