#pragma once
#include "fvCFD.H"  // Core header file of OpenFOAM
#include <petscksp.h>

namespace PETSc4FOAM {

    /**
     * @brief Encapsulation of the PETSc KSP linear solver
     */
    class KSPWrapper {
    public:
        /**
         * @brief Constructor (initialized with existing matrix and vectors)
         * @param A PETSc matrix
         * @param b Right-hand side vector
         * @param x Solution vector
         */
        KSPWrapper(Mat A, Vec b, Vec x);

        /// Configure solver parameters from an OpenFOAM dictionary
        void configureFromDict(const dictionary& dict);

        /// Execute the solving process
        void solve();

        /// Get the current residual norm
        PetscReal getResidualNorm() const;

        /// Destructor (automatically cleans up PETSc objects)
        ~KSPWrapper();

    private:
        KSP ksp_;       // PETSc solver context
        Mat A_;         // Coefficient matrix
        Vec b_, x_;     // Right-hand side and solution vectors
        PetscReal tol_; // Convergence tolerance
        PetscInt maxIters_; // Maximum number of iterations
    };

} // namespace PETSc4FOAM
