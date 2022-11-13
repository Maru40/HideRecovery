/*!
@file HasBallTransition.h
@brief HasBallTransition�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------

	class ItemBag;

	namespace maru {

		namespace Behavior {

			namespace Decorator {

				class HasBallTransition : public DecoratorBase<GameObject>
				{
					std::weak_ptr<ItemBag> m_itemBag;

				public:
					HasBallTransition(const std::shared_ptr<GameObject>& owner);

					bool CanTransition() const override;

					bool CanUpdate() override;

				};

			}
		}
	}
}