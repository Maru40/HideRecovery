/*!
@file BehaviorTask_Shot.cpp
@brief BehaviorTask_Shot‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "BehaviorTask_Shot.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Itabashi/OnlinePlayerSynchronizer.h"

#include "Maruyama/Player/Component/UseWeapon.h"
#include "Maruyama/Player/Component/WeponBase.h"
#include "Maruyama/Utility/Component/TargetManager.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Task {

				Shot::Shot(const std::shared_ptr<Enemy::EnemyBase>& owner):
					TaskBase(owner)
				{
					auto object = owner->GetGameObject();

					m_useWeapon = object->GetComponent<UseWeapon>(false);
					m_targetManager = object->GetComponent<TargetManager>(false);
					m_onlineSynchronizer = object->GetComponent<OnlinePlayerSynchronizer>(false);
				}

				void Shot::OnStart() {
					m_useWeapon.lock()->SetIsAim(true);
				}

				bool Shot::OnUpdate() {
					ShotUpdate();

					return false;
				}

				void Shot::OnExit() {

				}

				void Shot::ShotUpdate() {
					auto ownerTrans = GetOwner()->GetGameObject()->GetComponent<Transform>();
					m_useWeapon.lock()->GetWepon()->Shot(ownerTrans->GetForward());

					//m_onlineSynchronizer.lock()->Shot();
				}

			}
		}
	}
}