/*!
@file HasBallTransition.h
@brief HasBallTransition�Ȃ�
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

	namespace Enemy {
		class EnemyBase;
	}

	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//--------------------------------------------------------------------------------------
				/// ���̃����o�[���{�[���������Ă��鎞�̂ݑJ�ډ\
				//--------------------------------------------------------------------------------------
				class HasBall_OtherMember : public DecoratorBase<Enemy::EnemyBase>
				{
					std::weak_ptr<ItemBag> m_itemBag;

				public:
					HasBall_OtherMember(const std::shared_ptr<Enemy::EnemyBase>& owner);

					bool CanTransition() const override;

					bool CanUpdate() override;
				};

			}
		}
	}
}