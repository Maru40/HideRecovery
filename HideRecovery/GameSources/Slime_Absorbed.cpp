
/*!
@file Slime_Absorbed.cpp
@brief Slime_Absorbed�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyBase.h"
#include "Slime_Absorbed.h"

#include "VelocityManager.h"

#include "MetaballRenderBase.h"
#include "MetaballChildrenRender.h"

#include "Targeted.h"

#include "PressCenterManager.h"

#include "ShootManager.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			Slime_Absorbed::Slime_Absorbed(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr)
				:EnemyStateNodeBase(owner), m_paramPtr(paramPtr)
			{
				//�R���|�[�l���g�̐؂�ւ��ݒ�
				auto ownerObject = owner->GetGameObject();
				AddChangeComponent(ownerObject->GetComponent<CollisionObb>(false), false, true);
				AddChangeComponent(ownerObject->GetComponent<Gravity>(false), false, true);
				AddChangeComponent(ownerObject->GetComponent<PressCenterManager>(false), false, true);
				AddChangeComponent(ownerObject->GetComponent<Targeted>(false), false, true);
				AddChangeComponent(ownerObject->GetComponent<ShootSlime::ShootManager>(false), false, true);
			}

			void Slime_Absorbed::OnStart() {
				StartChangeComponents();

				auto velocityManager = GetOwner()->GetGameObject()->GetComponent<VelocityManager>(false);
				if (velocityManager) {
					velocityManager->ResetAll();
				}
			}

			bool Slime_Absorbed::OnUpdate() {
				return true;
			}

			void Slime_Absorbed::OnExit() {
				ExitChangeComponents();
			}

			//--------------------------------------------------------------------------------------
			/// �A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			std::shared_ptr<GameObject> Slime_Absorbed::GetParent() const {
				return GetOwner()->GetGameObject()->GetParent();
			}

		}
	}
}