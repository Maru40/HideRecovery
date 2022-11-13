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
		class I_FactionMember;
		
		namespace Tuple {
			class I_Tupler;
		}
	}

	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//--------------------------------------------------------------------------------------
				/// ���݂̃^�[�Q�b�g�����݂��^�[�Q�b�g�w��\���ǂ����𔻒f
				//--------------------------------------------------------------------------------------
				class CanCurrentTarget : public DecoratorBase<GameObject>
				{
					std::weak_ptr<TargetManager> m_targetManager;
					std::weak_ptr<Enemy::I_FactionMember> m_factionMember;
					std::weak_ptr<Enemy::Tuple::I_Tupler> m_tupler;

				public:
					CanCurrentTarget(const std::shared_ptr<GameObject>& owner);

					void ReserveCanTransition() override;

					bool CanTransition() const override;

					bool CanUpdate() override;

				private:
					void RemoveFactionTarget(const std::shared_ptr<GameObject>& target);

				};

			}
		}
	}
}