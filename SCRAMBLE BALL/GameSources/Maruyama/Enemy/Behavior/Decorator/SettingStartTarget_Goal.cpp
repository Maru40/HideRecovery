/*!
@file SettingStartTarget_Goal.cpp
@brief SettingStartTarget_Goal‚È‚ÇŽÀ‘Ì
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

#include "SettingStartTarget_Goal.h"
#include "Maruyama/StageObject/Goal.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Decorator {

				SettingStartTarget_Goal::SettingStartTarget_Goal(const std::shared_ptr<GameObject>& owner):
					DecoratorBase(owner)
				{
					m_teamMember = owner->GetComponent<I_TeamMember>(false);
					m_targetManager = owner->GetComponent<TargetManager>(false);
				}

				void SettingStartTarget_Goal::OnStart() {
					auto targets = ShareClassesManager::GetInstance()->GetCastShareClasses<Goal>();

					for (auto& target : targets) {
						if (target.lock()->GetTeam() != m_teamMember.lock()->GetTeam()) {
							m_targetManager.lock()->SetTarget(target.lock()->GetGameObject());
							break;
						}
					}
				}

			}
		}
	}
}