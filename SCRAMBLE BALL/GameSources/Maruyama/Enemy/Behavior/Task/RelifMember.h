/*!
@file RelifMember.h
@brief RelifMemberÇ»Ç«
íSìñé“ÅFä€éR óTäÏ
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
	class OnlinePlayerSynchronizer;

	struct Task_MovePositions_Parametor;

	namespace Enemy {
		class EnemyBase;
		class I_FactionMember;
	}

	namespace Task {
		struct MoveAstar_Parametor;
		struct ToTargetMove_Parametor;
		struct Wait_Parametor;
	}

	namespace TaskListNode {
		struct TargetSeek_Parametor;
	}

	namespace maru {

		namespace Behavior {

			namespace Task {

				class RelifMember : public TaskBase<Enemy::EnemyBase>
				{
				public:
					RelifMember(const std::shared_ptr<Enemy::EnemyBase>& owner);

					void OnStart() override;

					bool OnUpdate() override;

					void OnExit() override;
				};

			}
		}
	}
}