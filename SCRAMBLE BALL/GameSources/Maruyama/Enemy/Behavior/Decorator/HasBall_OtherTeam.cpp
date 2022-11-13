/*!
@file HasBallTransition.cpp
@brief HasBallTransitionなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

#include "HasBall_OtherTeam.h"

#include "Maruyama/Interface/I_TeamMember.h"
#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Enemy/AIDirector/CoordinatorBase.h"

#include "Maruyama/Player/Component/ItemBag.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/Utility/SingletonComponent/ShareClassesManager.h"

#include "Maruyama/Player/Object/PlayerObject.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Decorator {

				HasBall_OtherTeam::HasBall_OtherTeam(const std::shared_ptr<Enemy::EnemyBase>& owner) :
					DecoratorBase(owner)
				{
					auto object = owner->GetGameObject();

					m_itemBag = object->GetComponent<ItemBag>(false);
					m_teamMember = object->GetComponent<I_TeamMember>(false);
				}

				bool HasBall_OtherTeam::CanTransition() const {
					//自分自身がボールを持っているなら
					if (m_itemBag.lock()->GetHideItem()) {
						return false;	//遷移しない。
					}

					auto shareClassManager = ShareClassesManager::GetInstance();
					for (auto& weakMember : shareClassManager->GetCastShareClasses<PlayerObject>()) {
						auto member = weakMember.lock();
						if (member == GetOwner()->GetGameObject()) {	//自分自身なら処理を飛ばす。
							continue;
						}

						//同じチームなら処理を飛ばす。
						auto teamMember = member->GetComponent<I_TeamMember>(false);
						if (!teamMember) {
							continue;
						}
						if (teamMember->GetTeam() == m_teamMember.lock()->GetTeam()) {
							continue;
						}

						auto itemBag = member->GetComponent<ItemBag>(false);
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

				bool HasBall_OtherTeam::CanUpdate() {
					return CanTransition();
				}

			}
		}
	}
}