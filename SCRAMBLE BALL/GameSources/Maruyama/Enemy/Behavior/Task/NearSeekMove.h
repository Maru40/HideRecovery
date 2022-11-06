/*!
@file NearSeekMove.h
@brief NearSeekMoveなど
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
		//class I_FactionMember;
	}

	//namespace Task {
	//	struct ToTargetMove_Parametor;
	//}

	namespace TaskListNode {
		struct TargetSeek_Parametor;
	}

	namespace maru {

		namespace Behavior {

			namespace Task {

				//--------------------------------------------------------------------------------------
				///	ターゲットに近づくパラメータ
				//--------------------------------------------------------------------------------------
				struct NearSeekMove_Parametor {
					TaskListNode::TargetSeek_Parametor* moveParamPtr;

					NearSeekMove_Parametor();

					~NearSeekMove_Parametor();
				};

				//--------------------------------------------------------------------------------------
				///	ターゲットに近づくタスク
				//--------------------------------------------------------------------------------------
				class NearSeekMove : public TaskBase<Enemy::EnemyBase>
				{
				public:
					using Parametor = NearSeekMove_Parametor;

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
					NearSeekMove(const std::shared_ptr<Enemy::EnemyBase>& owner, const Parametor* paramPtr);

					virtual ~NearSeekMove() = default;

					void OnStart() override;

					bool OnUpdate() override;

					void OnExit() override;

				private:
					void SelectTask();

					void DefineTask();

					void RotationUpdate();

				public:

					bool IsEnd();
				};

			}
		}
	}
}