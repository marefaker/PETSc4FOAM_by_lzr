# 定位OpenFOAM核心头文件路径（如fvCFD.H）
find_path(OPENFOAM_INCLUDE_DIR
    NAMES fvCFD.H
    PATHS /opt/openfoam12/src/OpenFOAM
    NO_DEFAULT_PATH
    DOC "OpenFOAM核心头文件路径"
)

# 定位OpenFOAM核心库
find_library(OPENFOAM_CORE_LIB
    NAMES OpenFOAM
    PATHS /opt/openfoam12/platforms/linux64GccDPInt32Opt/lib
    DOC "OpenFOAM核心库路径"
)

# 定位finiteVolume等关键模块库
find_library(OPENFOAM_FINITEVOLUME_LIB
    NAMES finiteVolume
    PATHS /opt/openfoam12/platforms/linux64GccDPInt32Opt/lib
    DOC "OpenFOAM有限体积库路径"
)

# 合并库列表
set(OPENFOAM_LIBRARIES
    ${OPENFOAM_CORE_LIB}
    ${OPENFOAM_FINITEVOLUME_LIB}
)

# 验证是否找到所有依赖
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenFOAM
    REQUIRED_VARS 
        OPENFOAM_INCLUDE_DIR
        OPENFOAM_LIBRARIES
    FAIL_MESSAGE "未找到OpenFOAM安装，请检查路径或运行scripts/setup_dependencies.sh"
)