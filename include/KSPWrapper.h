#pragma once
#include "fvCFD.H"  // OpenFOAM核心头文件
#include <petscksp.h>

namespace PETSc4FOAM {

    / ​ * *​
        * @brief 封装PETSc KSP线性求解器
        * /
        class KSPWrapper {
        public:
            /* *​
                * @brief 构造函数（从现有矩阵和向量初始化）
                * @param A PETSc矩阵
                * @param b 右端项向量
                * @param x 解向量
                */
                KSPWrapper(Mat A, Vec b, Vec x);

            /// 从OpenFOAM字典配置求解器参数
            void configureFromDict(const dictionary& dict);

            /// 执行求解过程
            void solve();

            /// 获取当前残差范数
            PetscReal getResidualNorm() const;

            /// 析构函数（自动清理PETSc对象）
            ~KSPWrapper();

        private:
            KSP ksp_;       // PETSc求解器上下文
            Mat A_;         // 系数矩阵
            Vec b_, x_;     // 右端项和解向量
            PetscReal tol_; // 收敛容差
            PetscInt maxIters_; // 最大迭代次数
    };

} // namespace PETSc4FOAM