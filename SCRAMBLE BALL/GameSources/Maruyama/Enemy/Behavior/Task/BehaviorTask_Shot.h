/*!
@file BehaviorTask_Shot.h
@brief BehaviorTask_Shot�Ȃ�
�S���ҁF�ێR �T��
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
					std::weak_ptr<UseWeapon> m_useWeapon;							//����̎g�p
					std::weak_ptr<TargetManager> m_targetManager;					//�^�[�Q�b�g�Ǘ�
					std::weak_ptr<OnlinePlayerSynchronizer> m_onlineSynchronizer;	//�I�����C���V���N��

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