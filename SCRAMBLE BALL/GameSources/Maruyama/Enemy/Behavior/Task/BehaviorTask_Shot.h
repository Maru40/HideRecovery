/*!
@file BehaviorTask_Shot.h
@brief BehaviorTask_Shotなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Behavior/Interface/I_Task.h"

namespace basecross {

	class OnlinePlayerSynchronizer;
	class TargetManager;
	class UseWeapon;

	namespace Enemy {
		class EnemyBase;
		class I_FactionMember;
	}

	namespace Task {

	}

	namespace TaskListNode {
		
	}

	namespace maru {

		namespace Behavior {

			namespace Task {

				class Shot : public TaskBase<Enemy::EnemyBase>
				{
					std::weak_ptr<UseWeapon> m_useWeapon;							//武器の使用
					std::weak_ptr<TargetManager> m_targetManager;					//ターゲット管理
					std::weak_ptr<OnlinePlayerSynchronizer> m_onlineSynchronizer;	//オンラインシンクロ

				public:
					Shot(const std::shared_ptr<Enemy::EnemyBase>& owner);

					virtual ~Shot() = default;

					void OnStart() override;

					bool OnUpdate() override;

					void OnExit() override;

				private:
					void ShotUpdate();

				};

			}
		}
	}
}