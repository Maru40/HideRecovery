/*!
@file ObserveTargets.h
@brief ObserveTargets�Ȃ�
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
	class EyeSearchRange;
	class TargetManager;
	class GameTimer;

	class ObserveIsInEyeTarget;
	class I_TeamMember;

	namespace Enemy {
		class EnemyBase;
		class I_FactionMember;
	}

	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//--------------------------------------------------------------------------------------
				/// ����̃^�[�Q�b�g�̎������A�J�ڂ����O���鏈��
				//--------------------------------------------------------------------------------------
				class OutSpecificTarget : public DecoratorBase<Enemy::EnemyBase> 
				{
				public:
					OutSpecificTarget(const std::shared_ptr<Enemy::EnemyBase>& owner);

					bool CanTransition() const override;

					bool CanUpdate() override;
				};

			}
		}
	}
}