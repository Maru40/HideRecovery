/*!
@file HasBallTransition.h
@brief HasBallTransitionなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class ItemBag;

	namespace Enemy {
		class EnemyBase;
	}

	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//--------------------------------------------------------------------------------------
				/// 他のメンバーがボールを持っている時のみ遷移可能
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