#include "KSPWrapper.h"
#include "Foam2PETSc.h"
#include "ParallelUtilities.h"

using namespace PETSc4FOAM;

// ���캯��
KSPWrapper::KSPWrapper(Mat A, Vec b, Vec x)
    : A_(A), b_(b), x_(x), tol_(1e-6), maxIters_(1000) {
    KSPCreate(PETSC_COMM_WORLD, &ksp_);
    KSPSetOperators(ksp_, A_, A_); // ���þ�������������ã�
}

// ���ֵ����ò���
void KSPWrapper::configureFromDict(const dictionary& dict) {
    // ��ȡ��������
    tol_ = dict.lookupOrDefault<scalar>("tolerance", 1e-6);
    maxIters_ = dict.lookupOrDefault<int>("maxIterations", 1000);

    // �������������
    const word solverType = dict.lookupOrDefault<word>("ksp_type", "gmres");
    KSPSetType(ksp_, solverType.c_str());

    // ����Ԥ������
    const word pcType = dict.lookupOrDefault<word>("pc_type", "none");
    PC pc;
    KSPGetPC(ksp_, &pc);
    PCSetType(pc, pcType.c_str());

    // �����߼�ѡ���������������
    if (solverType == "gmres") {
        const int restart = dict.lookupOrDefault<int>("restart", 30);
        KSPGMRESSetRestart(ksp_, restart);
    }

    // Ӧ�ò�����PETSc
    KSPSetTolerances(ksp_, tol_, PETSC_DEFAULT, PETSC_DEFAULT, maxIters_);
    KSPSetFromOptions(ksp_); // ���������и��ǲ���
}

// ִ�����
void KSPWrapper::solve() {
    KSPSolve(ksp_, b_, x_);

    // ���������
    KSPConvergedReason reason;
    KSPGetConvergedReason(ksp_, &reason);

    if (reason < 0) {
        PetscInt iters;
        KSPGetIterationNumber(ksp_, &iters);
        PetscReal residual;
        KSPGetResidualNorm(ksp_, &residual);

        FatalErrorInFunction
            << "PETSc solver failed after " << iters << " iterations\n"
            << "  Residual: " << residual
            << "  Converged reason code: " << reason
            << exit(FatalError);
    }
}

// ��ȡ�в��
PetscReal KSPWrapper::getResidualNorm() const {
    PetscReal norm;
    KSPGetResidualNorm(ksp_, &norm);
    return norm;
}

// ��������
KSPWrapper::~KSPWrapper() {
    KSPDestroy(&ksp_);
}