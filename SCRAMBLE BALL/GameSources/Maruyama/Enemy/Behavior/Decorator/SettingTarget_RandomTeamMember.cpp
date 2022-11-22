/*!
@file SettingTarget_HasBallOtherMember.cpp
@brief SettingTarget_HasBallOtherMember‚È‚ÇŽÀ‘Ì
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

#include "SettingTarget_RandomTeamMember.h"

#include "Maruyama/Utility/Random.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Decorator {

				SettingTarget_RandomTeamMember::SettingTarget_RandomTeamMember(const std::shared_ptr<Enemy::EnemyBase>& owner):
					DecoratorBase(owner)
				{
					auto object = owner->GetGameObject();

					m_targetManager = object->GetComponent<TargetManager>(false);
				}

				void SettingTarget_RandomTeamMember::OnStart() {
					m_targetManager.lock()->SetTarget(SearchTarget());
				}

				std::shared_ptr<GameObject> SettingTarget_RandomTeamMember::SearchTarget() const {
					auto faction = GetOwner()->GetFactionCoordinator();
					if (!faction) {
						return nullptr;
					}

					auto member = maru::MyRandom::RandomArray(faction->GetMembers());

					return member.lock()->GetGameObject();
				}

			}
		}
	}
}