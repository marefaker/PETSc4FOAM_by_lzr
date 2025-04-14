#include "Foam2PETSc.h"
#include "ParallelUtilities.h"

using namespace PETSc4FOAM;

// ������תPETSc����
Vec Foam2PETSc::volScalarFieldToVec(const volScalarField& field) {
    // ��ȡȫ������ӳ��
    ISLocalToGlobalMapping mapping;
    Parallel::createGlobalIndexMapping(field.mesh(), &mapping);

    // ����PETSc����
    Vec vec;
    VecCreateMPI(PETSC_COMM_WORLD, field.size(), PETSC_DECIDE, &vec);
    VecSetLocalToGlobalMapping(vec, mapping);

    // ��ȡ��������ָ��
    PetscScalar* vecValues;
    VecGetArray(vec, &vecValues);

    // ������ݣ����������洢��
    const scalar* fieldData = field.internalField().cdata();
    std::copy(fieldData, fieldData + field.size(), vecValues);

    VecRestoreArray(vec, &vecValues);
    return vec;
}

// LDU����תAIJ����
Mat Foam2PETSc::lduMatrixToAIJMat(const lduMatrix& lduMat, bool symmetric) {
    const fvMesh& mesh = refCast<const fvMesh>(lduMat.psi().mesh());
    const labelUList& owner = lduMat.lduAddr().ownerAddr();
    const labelUList& neighbour = lduMat.lduAddr().neighbourAddr();

    // ��ȡȫ�ֵ�ԪIDӳ��
    ISLocalToGlobalMapping mapping;
    Parallel::createGlobalIndexMapping(mesh, &mapping);

    // ����AIJ����
    Mat mat;
    MatCreateAIJ(PETSC_COMM_WORLD,
        mesh.nCells(), mesh.nCells(),  // �������гߴ�
        PETSC_DETERMINE, PETSC_DETERMINE,
        0, nullptr, 0, nullptr, &mat);  // Ԥ�������Ԫ���Ż�

    // ������Ԫ��
    fillMatrixNonzeros(mat, lduMat, mesh.globalData().globalCellNumber());

    // ���þ�������
    MatSetOption(mat, MAT_SYMMETRIC, symmetric ? PETSC_TRUE : PETSC_FALSE);
    MatAssemblyBegin(mat, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(mat, MAT_FINAL_ASSEMBLY);

    return mat;
}

// �ڲ������������Ԫ��
void Foam2PETSc::fillMatrixNonzeros(
    Mat mat,
    const lduMatrix& lduMat,
    const labelList& globalCellIDs
) {
    const scalarField& diag = lduMat.diag();
    const scalarField& upper = lduMat.upper();
    const scalarField& lower = lduMat.lower();

    // ���Խ���Ԫ��
    forAll(diag, cellI) {
        PetscInt row = globalCellIDs[cellI];
        MatSetValue(mat, row, row, diag[cellI], INSERT_VALUES);
    }

    // �����������Ԫ��
    forAll(upper, faceI) {
        const PetscInt row = globalCellIDs[owner[faceI]];
        const PetscInt col = globalCellIDs[neighbour[faceI]];

        MatSetValue(mat, row, col, upper[faceI], INSERT_VALUES);
        MatSetValue(mat, col, row, lower[faceI], INSERT_VALUES);
    }
}