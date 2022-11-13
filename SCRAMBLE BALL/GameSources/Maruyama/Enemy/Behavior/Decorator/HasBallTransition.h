/*!
@file HasBallTransition.h
@brief HasBallTransition‚È‚Ç
’S“–ÒFŠÛR —TŠì
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// ‘O•ûéŒ¾
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