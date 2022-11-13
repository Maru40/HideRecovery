/*!
@file HasBall_OtherTeam.h
@brief HasBall_OtherTeamなど
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
	class I_TeamMember;

	namespace Enemy {
		class EnemyBase;
	}

	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//--------------------------------------------------------------------------------------
				/// 他のチームがボールを持っている時のみ遷移可能
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