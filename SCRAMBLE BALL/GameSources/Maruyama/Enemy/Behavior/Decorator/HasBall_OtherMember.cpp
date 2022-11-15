/*!
@file HasBallTransition.cpp
@brief HasBallTransition�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

#include "HasBall_OtherMember.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Enemy/AIDirector/CoordinatorBase.h"
#include "Maruyama/Enemy/AIDirector/FactionCoordinator.h"

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
					//�������g���{�[���������Ă���Ȃ�
					if (m_itemBag.lock()->GetHideItem()) {
						return false;	//�J�ڂ��Ȃ��B
					}

					auto faction = GetOwner()->GetFactionCoordinator();
					if (!faction) {
						return false;
					}

					for (auto& weakMember : faction->GetMembers()) {
						auto member = weakMember.lock();
						if (member == GetOwner()) {	//�������g�Ȃ珈�����΂��B
							continue;	
						}

						auto itemBag = member->GetGameObject()->GetComponent<ItemBag>(false);
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

				bool HasBall_OtherMember::CanUpdate() {
					return CanTransition();
				}

			}
		}
	}
}