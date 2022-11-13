/*!
@file HasBallTransition.cpp
@brief HasBallTransition�Ȃǎ���
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
					//�������g���{�[���������Ă���Ȃ�
					if (m_itemBag.lock()->GetHideItem()) {
						return false;	//�J�ڂ��Ȃ��B
					}

					auto shareClassManager = ShareClassesManager::GetInstance();
					for (auto& weakMember : shareClassManager->GetCastShareClasses<PlayerObject>()) {
						auto member = weakMember.lock();
						if (member == GetOwner()->GetGameObject()) {	//�������g�Ȃ珈�����΂��B
							continue;
						}

						//�����`�[���Ȃ珈�����΂��B
						auto teamMember = member->GetComponent<I_TeamMember>(false);
						if (!teamMember) {
							continue;
						}
						if (teamMember->GetTeam() == m_teamMember.lock()->GetTeam()) {
							continue;
						}

						auto itemBag = member->GetComponent<ItemBag>(false);
						if (!itemBag) {	//�A�C�e���������Ă��Ȃ��Ȃ珈�����΂��B
							continue;
						}

						auto hideItem = itemBag->GetHideItem();
						if (hideItem) {	//�B���A�C�e���������Ă���Ȃ�A�J�ډ\
							return true;
						}
					}

					return false;	//�N�������Ă��Ȃ����߁Afalse
				}

				bool HasBall_OtherTeam::CanUpdate() {
					return CanTransition();
				}

			}
		}
	}
}