#include "ParallelUtilities.h"

void PETSc4FOAM::Parallel::safeMPIInit(int* argc, char​** ​* argv) {
    int is_mpi_initialized;
    MPI_Initialized(&is_mpi_initialized);

    // 仅当MPI未初始化时执行
    if (!is_mpi_initialized) {
        MPI_Init(argc, argv);
        PetscInitialize(argc, argv, nullptr, nullptr);
    }

    // 确保OpenFOAM与PETSc使用相同通信子
    MPI_Comm_set_errhandler(PETSC_COMM_WORLD, MPI_ERRORS_RETURN);
}

void PETSc4FOAM::Parallel::validateMPIDomain(const fvMesh& mesh) {
    const int foamRank = Pstream::myProcNo();
    int petscRank;
    MPI_Comm_rank(PETSC_COMM_WORLD, &petscRank);

    if (foamRank != petscRank) {
        FatalErrorInFunction
            << "MPI进程号不一致：OpenFOAM进程 " << foamRank
            << " vs PETSc进程 " << petscRank
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
        1,  // 每个元素的块大小（标量场为1，矢量场为3）
        cellAddr.size(),
        globalIndices,
        PETSC_COPY_VALUES,
        mapping
    );

    delete[] globalIndices;
}