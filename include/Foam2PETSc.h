#pragma once
#include "fvCFD.H"  // Core header file of OpenFOAM
#include <petscvec.h>
#include <petscmat.h>

namespace PETSc4FOAM {

    /**
     * @brief A utility set for converting OpenFOAM data structures to PETSc
     */
    class Foam2PETSc {
    public:
        // Convert scalar field to PETSc vector (supports parallel domain decomposition)
        static Vec volScalarFieldToVec(const volScalarField& field);

        // Convert LDU matrix to PETSc AIJ matrix (automatically handles symmetry)
        static Mat lduMatrixToAIJMat(const lduMatrix& lduMat, bool symmetric = true);

        // Convert vector field to PETSc vector (flattened into a single column vector)
        static Vec volVectorFieldToVec(const volVectorField& field);

    private:
        // Internal: Handle the filling of non-zero elements in the matrix
        static void fillMatrixNonzeros(
            Mat mat,
            const lduMatrix& lduMat,
            const labelList& globalCellIDs
        );
    };

} // End namespace PETSc4FOAM
