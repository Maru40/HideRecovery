/*!
@file NearAstarMove.h
@brief NearAstarMoveなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Behavior/Interface/I_Task.h"

namespace basecross {

	template<class T>
	class TaskList;

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
				///	ターゲットにAstarで近づくパラメータ
				//--------------------------------------------------------------------------------------
				struct NearAstarMove_Parametor{
					basecross::Task::MoveAstar_Parametor* moveParamPtr;

					NearAstarMove_Parametor();

					~NearAstarMove_Parametor();
				};

				//--------------------------------------------------------------------------------------
				///	ターゲットにAstarで近づく
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

					bool IsEnd() const;
				};

			}
		}
	}
}