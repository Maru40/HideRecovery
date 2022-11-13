/*!
@file HasBall_OtherTeam.h
@brief HasBall_OtherTeam�Ȃ�
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

	class ItemBag;
	class I_TeamMember;

	namespace Enemy {
		class EnemyBase;
	}

	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//--------------------------------------------------------------------------------------
				/// ���̃`�[�����{�[���������Ă��鎞�̂ݑJ�ډ\
				//--------------------------------------------------------------------------------------
				class HasBall_OtherTeam : public DecoratorBase<Enemy::EnemyBase>
				{
					std::weak_ptr<ItemBag> m_itemBag;
					std::weak_ptr<I_TeamMember> m_teamMember;

				public:
					HasBall_OtherTeam(const std::shared_ptr<Enemy::EnemyBase>& owner);

					bool CanTransition() const override;

					bool CanUpdate() override;

				};

			}
		}
	}
}