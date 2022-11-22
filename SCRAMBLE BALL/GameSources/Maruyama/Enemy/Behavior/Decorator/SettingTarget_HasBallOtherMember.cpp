/*!
@file SettingTarget_HasBallOtherMember.cpp
@brief SettingTarget_HasBallOtherMemberなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/Interface/I_TeamMember.h"
#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/Utility/SingletonComponent/ShareClassesManager.h"

#include "Maruyama/Utility/Component/Targeted.h"
#include "Maruyama/Utility/Component/TargetManager.h"

#include "Maruyama/Enemy/AIDirector/FactionCoordinator.h"
#include "Maruyama/Player/Component/ItemBag.h"

#include "SettingTarget_HasBallOtherMember.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Decorator {

				SettingTarget_HasBallOtherMember::SettingTarget_HasBallOtherMember(const std::shared_ptr<Enemy::EnemyBase>& owner):
					DecoratorBase(owner)
				{
					auto object = owner->GetGameObject();

					m_targetManager = object->GetComponent<TargetManager>(false);
				}

				void SettingTarget_HasBallOtherMember::OnStart() {
					m_targetManager.lock()->SetTarget(SearchTarget());
				}

				std::shared_ptr<GameObject> SettingTarget_HasBallOtherMember::SearchTarget() const {
					auto faction = GetOwner()->GetFactionCoordinator();
					if (!faction) {
						return nullptr;
					}

					for (auto& weakMember : faction->GetMembers()) {
						auto member = weakMember.lock();
						if (member == GetOwner()) {	//自分自身なら処理を飛ばす。
							continue;
						}

						auto itemBag = member->GetGameObject()->GetComponent<ItemBag>(false);
						if (!itemBag) {	//アイテムを持っていないなら処理を飛ばす。
							continue;
						}

						auto hideItem = itemBag->GetHideItem();
						if (hideItem) {	//隠しアイテムを持っているなら、ターゲットにする。
							auto target = member->GetGameObject();
							return target;
						}
					}

					return nullptr;
				}

			}
		}
	}
}