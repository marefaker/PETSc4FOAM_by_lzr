#include "ParallelUtilities.h"

void PETSc4FOAM::Parallel::safeMPIInit(int* argc, char** argv) {
    int is_mpi_initialized;
    MPI_Initialized(&is_mpi_initialized);

    // Initialize MPI only if it is not already initialized
    if (!is_mpi_initialized) {
        MPI_Init(argc, argv);
        PetscInitialize(argc, argv, nullptr, nullptr);
    }

    // Ensure OpenFOAM and PETSc use the same communicator
    MPI_Comm_set_errhandler(PETSC_COMM_WORLD, MPI_ERRORS_RETURN);
}

void PETSc4FOAM::Parallel::validateMPIDomain(const fvMesh& mesh) {
    const int foamRank = Pstream::myProcNo();
    int petscRank;
    MPI_Comm_rank(PETSC_COMM_WORLD, &petscRank);

    if (foamRank != petscRank) {
        FatalErrorInFunction
            << "MPI process numbers do not match: OpenFOAM process " << foamRank
            << " vs PETSc process " << petscRank
            << abort(FatalError);
    }
}

void PETSc4FOAM::Parallel::createGlobalIndexMapping(
    const fvMesh& mesh,
    ISLocalToGlobalMapping* mapping
) {
    const labelList& cellAddr = mesh.globalData().globalCellNumber();
    PetscInt* globalIndices = new PetscInt[cellAddr.size()];

    forAll(cellAddr, i) {
        globalIndices[i] = cellAddr[i];
    }

    ISLocalToGlobalMappingCreate(
        PETSC_COMM_WORLD,
        1,  // Block size for each element (1 for scalar fields, 3 for vector fields)
        cellAddr.size(),
        globalIndices,
        PETSC_COPY_VALUES,
        mapping
    );

    delete[] globalIndices;
}
