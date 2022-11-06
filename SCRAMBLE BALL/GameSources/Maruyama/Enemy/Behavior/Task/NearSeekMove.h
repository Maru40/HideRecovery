/*!
@file Task_SearchBall.h
@brief Task_SearchBall�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Behavior/Interface/I_Task.h"

namespace basecross {

	template<class T>
	class TaskList;

	class TargetManager;
	class VelocityManager;
	class RotationController;
	class EyeSearchRange;

	namespace Enemy {
		class EnemyBase;
		class I_FactionMember;
	}

	namespace Task {
		struct ToTargetMove_Parametor;
	}

	namespace TaskListNode {
		struct TargetSeek_Parametor;
	}

	namespace maru {

		namespace Behavior {

			namespace Task {

				//--------------------------------------------------------------------------------------
				///	�^�[�Q�b�g�ɋ߂Â��p�����[�^
				//--------------------------------------------------------------------------------------
				struct NearSeekMove_Parametor {
					TaskListNode::TargetSeek_Parametor* moveParamPtr;

					NearSeekMove_Parametor();

					~NearSeekMove_Parametor();
				};

				//--------------------------------------------------------------------------------------
				///	�^�[�Q�b�g�ɋ߂Â��^�X�N
				//--------------------------------------------------------------------------------------
				class NearSeekMove : public TaskBase<Enemy::EnemyBase>
				{
				public:
					using Parametor = NearSeekMove_Parametor;

					enum class TaskEnum {
						Move,
					};

				private:
					Parametor m_param;

					std::unique_ptr<TaskList<TaskEnum>> m_taskList;

				public:
					NearSeekMove(const std::shared_ptr<Enemy::EnemyBase>& owner);

					virtual ~NearSeekMove() = default;

					void OnStart() override;

					bool OnUpdate() override;

					void OnExit() override;

				private:
					void SelectTask();

					void DefineTask();


				public:

					bool IsEnd();
				};

			}
		}
	}
}