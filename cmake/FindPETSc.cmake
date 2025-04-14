# ����ؼ�����
set(PETSC_DIR "$ENV{PETSC_DIR}" CACHE PATH "PETSc��Ŀ¼")
set(PETSC_ARCH "$ENV{PETSC_ARCH}" CACHE STRING "PETSc�ܹ�����")

# ��λPETScͷ�ļ�
find_path(PETSC_INCLUDE_DIR
    NAMES petsc.h petscvec.h
    HINTS "${PETSC_DIR}/include" 
          "${PETSC_DIR}/${PETSC_ARCH}/include"
    DOC "PETScͷ�ļ�·��"
)

# ��λ�ؼ���
find_library(PETSC_VEC_LIB
    NAMES petscvec
    HINTS "${PETSC_DIR}/${PETSC_ARCH}/lib"
    DOC "PETScʸ����"
)

find_library(PETSC_MAT_LIB
    NAMES petscmat
    HINTS "${PETSC_DIR}/${PETSC_ARCH}/lib"
    DOC "PETSc�����"
)

# �ϲ����ļ��б�
set(PETSC_LIBRARIES
    ${PETSC_VEC_LIB}
    ${PETSC_MAT_LIB}
    mpi  # ��Ҫ��ʽ����MPI��
)

# ��֤�Ƿ��������
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PETSc
    REQUIRED_VARS PETSC_INCLUDE_DIR PETSC_LIBRARIES
    VERSION_VAR PETSC_VERSION  # ��Ҫ�汾����߼�����ѡ��
)