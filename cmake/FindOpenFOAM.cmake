# ��λOpenFOAM����ͷ�ļ�·������fvCFD.H��
find_path(OPENFOAM_INCLUDE_DIR
    NAMES fvCFD.H
    PATHS /opt/openfoam12/src/OpenFOAM
    NO_DEFAULT_PATH
    DOC "OpenFOAM����ͷ�ļ�·��"
)

# ��λOpenFOAM���Ŀ�
find_library(OPENFOAM_CORE_LIB
    NAMES OpenFOAM
    PATHS /opt/openfoam12/platforms/linux64GccDPInt32Opt/lib
    DOC "OpenFOAM���Ŀ�·��"
)

# ��λfiniteVolume�ȹؼ�ģ���
find_library(OPENFOAM_FINITEVOLUME_LIB
    NAMES finiteVolume
    PATHS /opt/openfoam12/platforms/linux64GccDPInt32Opt/lib
    DOC "OpenFOAM���������·��"
)

# �ϲ����б�
set(OPENFOAM_LIBRARIES
    ${OPENFOAM_CORE_LIB}
    ${OPENFOAM_FINITEVOLUME_LIB}
)

# ��֤�Ƿ��ҵ���������
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenFOAM
    REQUIRED_VARS 
        OPENFOAM_INCLUDE_DIR
        OPENFOAM_LIBRARIES
    FAIL_MESSAGE "δ�ҵ�OpenFOAM��װ������·��������scripts/setup_dependencies.sh"
)