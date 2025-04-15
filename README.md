# PETSc4FOAM_by_lzr

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![OpenFOAM v12](https://img.shields.io/badge/OpenFOAM-v12-orange)](https://openfoam.org/)
[![PETSc 3.23](https://img.shields.io/badge/PETSc-3.23-brightgreen)](https://petsc.org/)

A bridge library for coupling OpenFOAM (v12.org) with PETSc 3.23.0 linear algebra backend.  
专为 OpenFOAM v12（.org发行版）与 PETSc 3.23.0 设计的线性代数后端对接库。

The program is in active development, and technical contributions are welcome.  
本项目处于开发阶段，欢迎技术贡献。

---

## Features / 核心功能

- **Seamless Data Conversion**  
  - Convert OpenFOAM fields (`scalarField/vectorField`) ↔ PETSc `Vec`  
  - Convert OpenFOAM `lduMatrix` <=> PETSc `Mat` (CSR/AIJ format)  

  **无缝数据转换**  
  - OpenFOAM 场数据 (`scalarField/vectorField`) ↔ PETSc `Vec`  
  - OpenFOAM `lduMatrix` <=> PETSc `Mat` (CSR/AIJ 格式)

- **Solver Abstraction**  
  - Preconfigured KSP solvers (GMRES/CG/BiCGSTAB)  
  - PC support (Jacobi/ILU/HYPRE) via `fvSolution` config  

  **求解器抽象层**  
  - 预配置 KSP 求解器 (GMRES/CG/BiCGSTAB)  
  - 通过 `fvSolution` 配置预条件子 (Jacobi/ILU/HYPRE)

- **Parallel Computing**  
  - Native MPI support with domain decomposition consistency  

  **并行计算**  
  - 原生 MPI 支持，保持域分解一致性

---

## Installation / 安装指南

### Prerequisites / 环境要求
- WSL2/Ubuntu 22.04+
- OpenFOAM v12 ([Official Guide](https://openfoam.org/download/12-ubuntu/))
- PETSc 3.23.0 ([Build Guide](https://petsc.org/release/install/))

### Compatibility / 适用性声明

- This project is designed for OpenFOAM v12 and PETSc 3.23.0. Compatibility with other versions or distributions (e.g., OpenFOAM.com) is not guaranteed.

- 本项目专为 OpenFOAM v12 和 PETSc 3.23.0 设计，可能无法适用于其他版本或发行版（如 OpenFOAM.com）。

### Quick Start / 快速安装
```bash
# Clone repository / 克隆仓库
git clone git@github.com:marefaker/PETSc4FOAM_by_lzr.git
cd PETSc4FOAM_by_lzr

# Install dependencies / 安装依赖项
chmod +x scripts/setup_dependencies.sh
./scripts/setup_dependencies.sh

# Build library / 构建库
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH="/opt/openfoam12;$PETSC_DIR/$PETSC_ARCH"
make -j4

# Optional: Install to system / 可选：安装至系统
sudo make install
```

---

### Usage / 使用示例

1. Modify `fvSolution` / 修改求解器配置

```yaml
solvers {
    p {
        solver          petsc;     # Use PETSc solver / 使用PETSc求解器
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
libs ("libPETSc4FOAM_by_lzr.so");

# Execute solver
mpirun -np 4 myFoamSolver -parallel
```

## Contributing / 贡献指南

- Contributions from experienced programmers are highly appreciated.  
  欢迎有经验的程序员贡献代码。

- If you find bugs or have suggestions, please contact the author.  
  如果发现漏洞或有建议，请联系作者。

## License / 许可证

- Distributed under the MIT License. See LICENSE for details.  
  本项目采用 MIT 许可证，详情见许可证文件。

## Contact / 联系方式

- Author: Zirui Liu / 刘子睿  
- Academy: Shanghai Jiao Tong University / 上海交通大学
- Email: meadamliu@sjtu.edu.cn or liua8334@gmail.com  

## Tittle-tattle / 题外话

- This is a personal study project, not affiliated with OpenFOAM or PETSc.  
  本项目是个人学习项目，与 OpenFOAM 或 PETSc 无关。

- The author is a mechanical engineering undergraduate with limited experience in C++ and OpenFOAM.  
  作者为机械工程本科生，对 C++ 和 OpenFOAM 经验有限。

- But my girlfriend majors software engineering (^v^).
  但是我的女朋友是学软件工程的（^v^）。