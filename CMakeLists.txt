cmake_minimum_required(VERSION 3.10)
project(PETSc4FOAM_by_lzr LANGUAGES CXX)

# 加载自定义模块
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(OpenFOAM REQUIRED)
find_package(PETSc REQUIRED)

# 设置C++标准（与OpenFOAM一致）
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 启用所有警告
add_compile_options(-Wall -Wextra -Wpedantic)

# 检查声明与定义一致性
add_compile_options(-Wmissing-declarations)  # 未声明的函数定义会警告
add_compile_options(-Wunused-function)       # 未使用的静态函数警告

# 添加双精度编译标志
add_definitions(-DWM_DP)

# 生成共享库
add_library(PETSc4FOAM_by_lzr SHARED
    src/Foam2PETSc.cpp
    src/KSPWrapper.cpp
    src/ParallelUtilities.cpp
)

# 包含路径
target_include_directories(PETSc4FOAM_by_lzr PUBLIC
    ${OPENFOAM_INCLUDE_DIR}
    ${PETSC_INCLUDE_DIRS}
    include/
)

# 链接库
target_link_libraries(PETSc4FOAM_by_lzr PUBLIC
    ${OPENFOAM_LIBRARIES}
    ${PETSC_LIBRARIES}
)

# 安装规则
install(TARGETS PETSc4FOAM_by_lzr DESTINATION lib)
install(DIRECTORY include/ DESTINATION include/PETSc4FOAM)