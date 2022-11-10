/*!
@file NearAstarMove.h
@brief NearAstarMove�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Behavior/Interface/I_Task.h"

namespace basecross {

	template<class T>
	class TaskList;

	struct EyeSearchRangeParametor;
	class EyeSearchRange;
	class TargetManager;
	class RotationController;

	namespace Enemy {
		class EnemyBase;
	}

	namespace Task {
		struct MoveAstar_Parametor;
	}

	namespace maru {

		namespace Behavior {

			namespace Task {

				//--------------------------------------------------------------------------------------
				///	�^�[�Q�b�g��Astar�ŋ߂Â��p�����[�^
				//--------------------------------------------------------------------------------------
				struct NearAstarMove_Parametor{
					EyeSearchRangeParametor* eyeSearchParamPtr;
					basecross::Task::MoveAstar_Parametor* moveParamPtr;

					NearAstarMove_Parametor();

					NearAstarMove_Parametor(const EyeSearchRangeParametor& eyeParam);

					~NearAstarMove_Parametor();
				};

				//--------------------------------------------------------------------------------------
				///	�^�[�Q�b�g��Astar�ŋ߂Â�
				//--------------------------------------------------------------------------------------
				class NearAstarMove : public TaskBase<Enemy::EnemyBase> 
				{
				public:
					using Parametor = NearAstarMove_Parametor;

					enum class TaskEnum {
						Move,
					};

				private:
					const Parametor* m_paramPtr;

					std::weak_ptr<EyeSearchRange> m_eyeRange;
					std::weak_ptr<TargetManager> m_targetManager;
					std::weak_ptr<RotationController> m_rotationController;

					std::unique_ptr<TaskList<TaskEnum>> m_taskList;

				public:
					NearAstarMove(const std::shared_ptr<Enemy::EnemyBase>& owner, const Parametor* paramPtr);

					virtual ~NearAstarMove() = default;

					void OnStart() override;

					bool OnUpdate() override;

					void OnExit() override;

				private:
					void DefineTask();

					void SelectTask();

					void RotationUpdate();

					/// <summary>
					/// �I������
					/// </summary>
					/// <returns>�^�[�Q�b�g�����Ȃ��A�������̓^�[�Q�b�g�����E���Ȃ�I������B</returns>
					bool IsEnd() const;
				};

			}
		}
	}
}