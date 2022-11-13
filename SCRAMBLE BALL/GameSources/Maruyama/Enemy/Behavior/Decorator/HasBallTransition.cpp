/*!
@file HasBallTransition.cpp
@brief HasBallTransition�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

#include "HasBallTransition.h"

#include "Maruyama/Player/Component/ItemBag.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Decorator {

				HasBallTransition::HasBallTransition(const std::shared_ptr<GameObject>& owner):
					DecoratorBase(owner)
				{
					m_itemBag = owner->GetComponent<ItemBag>(false);
				}

				bool HasBallTransition::CanTransition() const {
					//�B���A�C�e���������Ă���Ȃ�true
					auto hideItem = m_itemBag.lock()->GetHideItem();
					return hideItem ? true : false;
				}

				bool HasBallTransition::CanUpdate() {
					return CanTransition();
				}

			}
		}
	}
}