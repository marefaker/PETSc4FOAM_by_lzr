#pragma once
#include "fvCFD.H"  // OpenFOAM核心头文件
#include <petscvec.h>
#include <petscmat.h>

namespace PETSc4FOAM {

    /* *​
        * @brief OpenFOAM数据结构与PETSc的转换工具集
        */
        class Foam2PETSc {
        public:
            // 标量场转PETSc向量（支持并行域分解）
            static Vec volScalarFieldToVec(const volScalarField& field);

            // LDU矩阵转PETSc AIJ矩阵（自动处理对称性）
            static Mat lduMatrixToAIJMat(const lduMatrix& lduMat, bool symmetric = true);

            // 矢量场转PETSc向量（展开为单列向量）
            static Vec volVectorFieldToVec(const volVectorField& field);

        private:
            // 内部：处理矩阵非零元素填充
            static void fillMatrixNonzeros(
                Mat mat,
                const lduMatrix& lduMat,
                const labelList& globalCellIDs
            );
    };

} // End namespace PETSc4FOAM