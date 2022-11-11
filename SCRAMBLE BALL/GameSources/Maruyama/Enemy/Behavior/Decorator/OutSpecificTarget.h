/*!
@file OutSpecificTarget.h
@brief OutSpecificTarget�Ȃ�
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
				/// ����̃^�[�Q�b�g�̎������A�J�ڂ����O���鏈��
				//--------------------------------------------------------------------------------------
				class OutSpecificTarget : public DecoratorBase<Enemy::EnemyBase> 
				{
					std::weak_ptr<GameObject> m_target;				//�^�[�Q�b�g

					std::weak_ptr<TargetManager> m_targetManager;	//�^�[�Q�b�g�Ǘ�

				public:
					OutSpecificTarget(
						const std::shared_ptr<Enemy::EnemyBase>& owner,
						const std::shared_ptr<GameObject>& target
					);

					bool CanTransition() const override;

					bool CanUpdate() override;

				public:
					//--------------------------------------------------------------------------------------
					/// �A�N�Z�b�T
					//--------------------------------------------------------------------------------------

					void SetTarget(const std::shared_ptr<GameObject>& target) { m_target = target; }

					_NODISCARD std::shared_ptr<GameObject> GetTarget() const noexcept { return m_target.lock(); }
				};

			}
		}
	}
}