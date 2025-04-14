#include "Foam2PETSc.h"
#include "ParallelUtilities.h"

using namespace PETSc4FOAM;

// 标量场转PETSc向量
Vec Foam2PETSc::volScalarFieldToVec(const volScalarField& field) {
    // 获取全局索引映射
    ISLocalToGlobalMapping mapping;
    Parallel::createGlobalIndexMapping(field.mesh(), &mapping);

    // 创建PETSc向量
    Vec vec;
    VecCreateMPI(PETSC_COMM_WORLD, field.size(), PETSC_DECIDE, &vec);
    VecSetLocalToGlobalMapping(vec, mapping);

    // 获取本地数据指针
    PetscScalar* vecValues;
    VecGetArray(vec, &vecValues);

    // 填充数据（假设连续存储）
    const scalar* fieldData = field.internalField().cdata();
    std::copy(fieldData, fieldData + field.size(), vecValues);

    VecRestoreArray(vec, &vecValues);
    return vec;
}

// LDU矩阵转AIJ矩阵
Mat Foam2PETSc::lduMatrixToAIJMat(const lduMatrix& lduMat, bool symmetric) {
    const fvMesh& mesh = refCast<const fvMesh>(lduMat.psi().mesh());
    const labelUList& owner = lduMat.lduAddr().ownerAddr();
    const labelUList& neighbour = lduMat.lduAddr().neighbourAddr();

    // 获取全局单元ID映射
    ISLocalToGlobalMapping mapping;
    Parallel::createGlobalIndexMapping(mesh, &mapping);

    // 创建AIJ矩阵
    Mat mat;
    MatCreateAIJ(PETSC_COMM_WORLD,
        mesh.nCells(), mesh.nCells(),  // 本地行列尺寸
        PETSC_DETERMINE, PETSC_DETERMINE,
        0, nullptr, 0, nullptr, &mat);  // 预分配非零元素优化

    // 填充矩阵元素
    fillMatrixNonzeros(mat, lduMat, mesh.globalData().globalCellNumber());

    // 设置矩阵属性
    MatSetOption(mat, MAT_SYMMETRIC, symmetric ? PETSC_TRUE : PETSC_FALSE);
    MatAssemblyBegin(mat, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(mat, MAT_FINAL_ASSEMBLY);

    return mat;
}

// 内部：填充矩阵非零元素
void Foam2PETSc::fillMatrixNonzeros(
    Mat mat,
    const lduMatrix& lduMat,
    const labelList& globalCellIDs
) {
    const scalarField& diag = lduMat.diag();
    const scalarField& upper = lduMat.upper();
    const scalarField& lower = lduMat.lower();

    // 填充对角线元素
    forAll(diag, cellI) {
        PetscInt row = globalCellIDs[cellI];
        MatSetValue(mat, row, row, diag[cellI], INSERT_VALUES);
    }

    // 填充上下三角元素
    forAll(upper, faceI) {
        const PetscInt row = globalCellIDs[owner[faceI]];
        const PetscInt col = globalCellIDs[neighbour[faceI]];

        MatSetValue(mat, row, col, upper[faceI], INSERT_VALUES);
        MatSetValue(mat, col, row, lower[faceI], INSERT_VALUES);
    }
}