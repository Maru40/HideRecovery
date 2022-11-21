/*!
@file SettingTarget_HasBallOtherMember.h
@brief SettingTarget_HasBallOtherMember�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------

	class TargetManager;

	namespace Enemy {
		class EnemyBase;
	}

	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//--------------------------------------------------------------------------------------
				/// �{�[���������Ă��鑼�̃����o�[��T���āA�^�[�Q�b�g�Ɏw�肷��R���|�[�l���g�B
				//--------------------------------------------------------------------------------------
				class SettingTarget_HasBallOtherMember : public DecoratorBase<Enemy::EnemyBase> 
				{
					std::weak_ptr<TargetManager> m_targetManager;

				public:
					SettingTarget_HasBallOtherMember(const std::shared_ptr<Enemy::EnemyBase>& owner);

					void OnStart() override;

					bool CanTransition() const override { return true; }

					bool CanUpdate() override { return true; }

				private:
					std::shared_ptr<GameObject> SearchTarget() const;
				};

			}
		}
	}
}