#pragma once
#include <mpi.h>
#include <petscsys.h>
#include "fvCFD.H"  // OpenFOAM核心头文件

namespace PETSc4FOAM {
	namespace Parallel {

		/* *​
			* @brief 安全初始化MPI环境，避免OpenFOAM与PETSc重复初始化
			* @param argc 命令行参数计数指针
			* @param argv 命令行参数数组指针
			*/
			void safeMPIInit(int* argc, char​ * *​ * argv);

		/* *​
			* @brief 验证OpenFOAM与PETSc的MPI通信域一致性
			* @param mesh OpenFOAM网格对象（用于获取域分解信息）
			*/
			void validateMPIDomain(const fvMesh & mesh);

		/* *​
			* @brief 创建OpenFOAM网格分区到PETSc全局索引的映射
			* @param mesh OpenFOAM网格对象
			* @param mapping 输出参数，PETSc全局索引映射对象
			*/
			void createGlobalIndexMapping(const fvMesh & mesh, ISLocalToGlobalMapping * mapping);

	}
}