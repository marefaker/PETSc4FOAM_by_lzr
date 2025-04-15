#include "Foam2PETSc.h"
#include "ParallelUtilities.h"

using namespace PETSc4FOAM;

// Convert scalar field to PETSc vector
Vec Foam2PETSc::volScalarFieldToVec(const volScalarField& field) {
    // Obtain global index mapping
    ISLocalToGlobalMapping mapping;
    Parallel::createGlobalIndexMapping(field.mesh(), &mapping);

    // Create PETSc vector
    Vec vec;
    VecCreateMPI(PETSC_COMM_WORLD, field.size(), PETSC_DECIDE, &vec);
    VecSetLocalToGlobalMapping(vec, mapping);

    // Get local data pointer
    PetscScalar* vecValues;
    VecGetArray(vec, &vecValues);

    // Fill data (assume continuous storage)
    const scalar* fieldData = field.internalField().cdata();
    std::copy(fieldData, fieldData + field.size(), vecValues);

    VecRestoreArray(vec, &vecValues);
    return vec;
}

// Convert LDU matrix to AIJ matrix
Mat Foam2PETSc::lduMatrixToAIJMat(const lduMatrix& lduMat, bool symmetric) {
    const fvMesh& mesh = refCast<const fvMesh>(lduMat.psi().mesh());
    const labelUList& owner = lduMat.lduAddr().ownerAddr();
    const labelUList& neighbour = lduMat.lduAddr().neighbourAddr();

    // Obtain global unit ID mapping
    ISLocalToGlobalMapping mapping;
    Parallel::createGlobalIndexMapping(mesh, &mapping);

    // Create AIJ matrix
    Mat mat;
    MatCreateAIJ(PETSC_COMM_WORLD,
        mesh.nCells(), mesh.nCells(),  // Local row and column sizes
        PETSC_DETERMINE, PETSC_DETERMINE,
        0, nullptr, 0, nullptr, &mat);  // Preallocate for non-zero element optimization

    // Fill matrix elements
    fillMatrixNonzeros(mat, lduMat, mesh.globalData().globalCellNumber());

    // Set matrix properties
    MatSetOption(mat, MAT_SYMMETRIC, symmetric ? PETSC_TRUE : PETSC_FALSE);
    MatAssemblyBegin(mat, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(mat, MAT_FINAL_ASSEMBLY);

    return mat;
}

// Internal: Fill non-zero matrix elements
void Foam2PETSc::fillMatrixNonzeros(
    Mat mat,
    const lduMatrix& lduMat,
    const labelList& globalCellIDs
) {
    const scalarField& diag = lduMat.diag();
    const scalarField& upper = lduMat.upper();
    const scalarField& lower = lduMat.lower();

    // Fill diagonal elements
    forAll(diag, cellI) {
        PetscInt row = globalCellIDs[cellI];
        MatSetValue(mat, row, row, diag[cellI], INSERT_VALUES);
    }

    // Fill upper and lower triangular elements
    forAll(upper, faceI) {
        const PetscInt row = globalCellIDs[owner[faceI]];
        const PetscInt col = globalCellIDs[neighbour[faceI]];

        MatSetValue(mat, row, col, upper[faceI], INSERT_VALUES);
        MatSetValue(mat, col, row, lower[faceI], INSERT_VALUES);
    }
}
