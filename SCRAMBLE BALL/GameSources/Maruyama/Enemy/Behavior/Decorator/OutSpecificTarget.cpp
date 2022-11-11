/*!
@file OutSpecificTarget.cpp
@brief OutSpecificTargetなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"
#include "OutSpecificTarget.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Utility/Component/TargetManager.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Decorator {

				OutSpecificTarget::OutSpecificTarget(
					const std::shared_ptr<Enemy::EnemyBase>& owner,
					const std::shared_ptr<GameObject>& target
				):
					DecoratorBase(owner),
					m_target(target)
				{
					auto object = owner->GetGameObject();

					m_targetManager = object->GetComponent<TargetManager>(false);
				}

				bool OutSpecificTarget::CanTransition() const {
					//コンポーネントnullチェック
					auto targetManager = m_targetManager.lock();
					if (!targetManager || !targetManager->HasTarget()) {
						return true;
					}

					//ターゲット管理と指定されたターゲットが同じなら、遷移できない
					if (targetManager->GetTarget() == GetTarget()) {
						return false;
					}

					return true;	//それ以外のターゲットなら遷移可能。
				}

				bool OutSpecificTarget::CanUpdate() {
					return CanTransition();
				}

			}
		}
	}
}