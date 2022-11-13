/*!
@file SettingStartTarget_Ball.cpp
@brief SettingStartTarget_Ball‚È‚ÇŽÀ‘Ì
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

#include "SettingStartTarget_Ball.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Decorator {

				SettingStartTarget_Ball::SettingStartTarget_Ball(
					const std::shared_ptr<GameObject>& owner,
					const std::shared_ptr<GameObject>& target
				) :
					DecoratorBase(owner),
					m_target(target)
				{
					m_targetManager = owner->GetComponent<TargetManager>(false);
				}

				void SettingStartTarget_Ball::OnStart() {
					m_targetManager.lock()->SetTarget(m_target.lock());
				}

				void SettingStartTarget_Ball::OnExit() {
					m_targetManager.lock()->SetTarget(nullptr);
				}

			}
		}
	}
}