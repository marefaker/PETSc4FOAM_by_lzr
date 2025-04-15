#pragma once
#include <mpi.h>
#include <petscsys.h>
#include "fvCFD.H"  // Core header file of OpenFOAM

namespace PETSc4FOAM {
	namespace Parallel {

		/**
		 * @brief Safely initialize the MPI environment, avoiding duplicate initialization by OpenFOAM and PETSc
		 * @param argc Pointer to the command-line argument count
		 * @param argv Pointer to the command-line argument array
		 */
		void safeMPIInit(int* argc, char*** argv);

		/**
		 * @brief Validate the consistency of the MPI communication domain between OpenFOAM and PETSc
		 * @param mesh OpenFOAM mesh object (used to obtain domain decomposition information)
		 */
		void validateMPIDomain(const fvMesh& mesh);

		/**
		 * @brief Create a mapping from OpenFOAM mesh partitions to PETSc global indices
		 * @param mesh OpenFOAM mesh object
		 * @param mapping Output parameter, PETSc global index mapping object
		 */
		void createGlobalIndexMapping(const fvMesh& mesh, ISLocalToGlobalMapping* mapping);

	}
}
