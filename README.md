# PETSc4FOAM_by_lzr

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![OpenFOAM v12](https://img.shields.io/badge/OpenFOAM-v12-orange)](https://openfoam.org/)
[![PETSc 3.23](https://img.shields.io/badge/PETSc-3.23-brightgreen)](https://petsc.org/)

A bridge library for coupling OpenFOAM (v12.org) with PETSc 3.23.0 linear algebra backend.  
专为 OpenFOAM v12（.org发行版）与 PETSc 3.23.0 设计的线性代数后端对接库

---

## Features / 核心功能

- ​**​Seamless Data Conversion​**​  
  - Convert OpenFOAM fields (`scalarField/vectorField`) ↔ PETSc `Vec`  
  - Convert OpenFOAM `lduMatrix` ↔ PETSc `Mat` (CSR/AIJ format)  
  ​**​无缝数据转换​**​  
  - OpenFOAM 场数据 (`scalarField/vectorField`) ↔ PETSc `Vec`  
  - OpenFOAM `lduMatrix` ↔ PETSc `Mat` (CSR/AIJ 格式)

- ​**​Solver Abstraction​**​  
  - Preconfigured KSP solvers (GMRES/CG/BiCGSTAB)  
  - PC support (Jacobi/ILU/HYPRE) via `fvSolution` config  
  ​**​求解器抽象层​**​  
  - 预配置 KSP 求解器 (GMRES/CG/BiCGSTAB)  
  - 通过 `fvSolution` 配置预条件子 (Jacobi/ILU/HYPRE)

- ​**​Parallel Computing​**​  
  - Native MPI support with domain decomposition consistency  
  ​**​并行计算​**​  
  - 原生 MPI 支持，保持域分解一致性

---

## Installation / 安装指南

### Prerequisites / 环境要求
- WSL2/Ubuntu 20.04+ or Linux OS
- OpenFOAM v12 ([Official Guide](https://openfoam.org/download/12-ubuntu/))
- PETSc 3.23.0 ([Build Guide](https://petsc.org/release/install/))

### Quick Start / 快速安装
```bash
# Clone repository
git clone https://github.com/yourusername/OpenFOAM-PETSc-Adapter.git
cd OpenFOAM-PETSc-Adapter

# Install dependencies (首次运行)
chmod +x scripts/setup_dependencies.sh
./scripts/setup_dependencies.sh  # 自动安装OpenFOAM和PETSc

# Build library
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH="/opt/openfoam12;$PETSC_DIR/$PETSC_ARCH"
make -j4

# Install to system (可选)
sudo make install
```

---


### Usage  / 使用示例

1. Modify fvSolution / 修改求解器配置

```yaml
solvers {
    p {
        solver          petsc;     // 使用PETSc求解器
        petscOptions    "-ksp_type gmres -pc_type hypre -ksp_monitor";
        tolerance       1e-8;
        maxIter         1000;
    }
}
```
2. Call in OpenFOAM Solver / 在求解器中调用

```cpp
#include "FoamToPETSc.H"

// In main solver function
Vec petscRHS = FoamToPETSc::fieldToVec(rhsField);
Mat petscMat = FoamToPETSc::lduMatrixToMat(lduMatrix);

PETScSolver solver(petscMat, petscRHS);
solver.solve();

scalarField solution = FoamToPETSc::vecToField(solver.getSolution());
```
3. Run Case / 运行算例
```bash
# Load library in controlDict
libs ("libOpenFOAM-PETSc-Adapter.so");

# Execute solver
mpirun -np 4 myFoamSolver -parallel
```
## Contributing / 贡献指南

  It can not be better if a professional programmer helps me.

  最好有科班出身的程序员帮我收拾这个烂摊子。

## License / 许可证

  Distributed under the MIT License. See LICENSE for details.

  本项目采用 MIT 许可证，详情见许可证文件。

## Contact / 联系方式

  Author: Zirui Liu / 刘子睿

  Email: meadamliu@sjtu.edu.cn or liua8334@gmail.com

## Tittle-tattle / 题外话

  This project is a personal study project, not an official OpenFOAM or PETSc project.

  这项目是哥们看没有针对.org发行版的接口才做的，和OpenFOAM或者PETSc没有关系。

  To be honest I am an undergraduate whose major is mechanical engineering, so I am not very familiar with C++ and OpenFOAM.

  哥们只是个本科生，学的还是机械工程，cpp依托勾史，OpenFOAM也只会拉包捏，就算反馈问题我也不一定能解决（lol）。

  但是我的女朋友是学软件的（^v^）。

  It is not suprising that mechanical engineering requires C++ but it IS that it is so torturous for me.

  人类在追求机械飞升的路上先倒在了赛博飞升这一关。
