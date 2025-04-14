#include "KSPWrapper.h"
#include "Foam2PETSc.h"
#include "ParallelUtilities.h"

using namespace PETSc4FOAM;

// 构造函数
KSPWrapper::KSPWrapper(Mat A, Vec b, Vec x)
    : A_(A), b_(b), x_(x), tol_(1e-6), maxIters_(1000) {
    KSPCreate(PETSC_COMM_WORLD, &ksp_);
    KSPSetOperators(ksp_, A_, A_); // 设置矩阵（允许后续重用）
}

// 从字典配置参数
void KSPWrapper::configureFromDict(const dictionary& dict) {
    // 读取基础参数
    tol_ = dict.lookupOrDefault<scalar>("tolerance", 1e-6);
    maxIters_ = dict.lookupOrDefault<int>("maxIterations", 1000);

    // 设置求解器类型
    const word solverType = dict.lookupOrDefault<word>("ksp_type", "gmres");
    KSPSetType(ksp_, solverType.c_str());

    // 设置预条件子
    const word pcType = dict.lookupOrDefault<word>("pc_type", "none");
    PC pc;
    KSPGetPC(ksp_, &pc);
    PCSetType(pc, pcType.c_str());

    // 其他高级选项（例如重启次数）
    if (solverType == "gmres") {
        const int restart = dict.lookupOrDefault<int>("restart", 30);
        KSPGMRESSetRestart(ksp_, restart);
    }

    // 应用参数到PETSc
    KSPSetTolerances(ksp_, tol_, PETSC_DEFAULT, PETSC_DEFAULT, maxIters_);
    KSPSetFromOptions(ksp_); // 允许命令行覆盖参数
}

// 执行求解
void KSPWrapper::solve() {
    KSPSolve(ksp_, b_, x_);

    // 检查收敛性
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

// 获取残差范数
PetscReal KSPWrapper::getResidualNorm() const {
    PetscReal norm;
    KSPGetResidualNorm(ksp_, &norm);
    return norm;
}

// 析构函数
KSPWrapper::~KSPWrapper() {
    KSPDestroy(&ksp_);
}