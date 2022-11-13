/*!
@file HasBallTransition.cpp
@brief HasBallTransitionなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

#include "HasBall_OtherMember.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Enemy/AIDirector/CoordinatorBase.h"

#include "Maruyama/Player/Component/ItemBag.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Decorator {

				HasBall_OtherMember::HasBall_OtherMember(const std::shared_ptr<Enemy::EnemyBase>& owner):
					DecoratorBase(owner)
				{
					auto object = owner->GetGameObject();

					m_itemBag = object->GetComponent<ItemBag>(false);
				}

				bool HasBall_OtherMember::CanTransition() const {
					//自分自身がボールを持っているなら
					if (m_itemBag.lock()->GetHideItem()) {
						return false;	//遷移しない。
					}

					auto assignedFaction = GetOwner()->GetAssignedFaction();
					if (!assignedFaction) {
						return false;
					}

					for (auto& weakMember : assignedFaction->GetMembers()) {
						auto member = weakMember.lock();
						if (member == GetOwner()) {	//自分自身なら処理を飛ばす。
							continue;	
						}

						auto itemBag = member->GetGameObject()->GetComponent<ItemBag>(false);
						if (!itemBag) {	//アイテムを持っていないなら処理を飛ばす。
							continue;
						}

						auto hideItem = itemBag->GetHideItem();
						if (hideItem) {	//隠しアイテムを持っているなら、遷移可能
							return true;
						}
					}

					return false;	//誰も持っていないため、false
				}

				bool HasBall_OtherMember::CanUpdate() {
					return CanTransition();
				}

			}
		}
	}
}