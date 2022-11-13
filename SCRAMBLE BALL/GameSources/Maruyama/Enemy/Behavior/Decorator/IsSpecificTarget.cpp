/*!
@file IsSpecificTarget.cpp
@brief IsSpecificTarget‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

#include "IsSpecificTarget.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Decorator {

				IsSpecificTarget::IsSpecificTarget(
					const std::shared_ptr<GameObject>& owner,
					const std::shared_ptr<GameObject>& target
				):
					DecoratorBase(owner),
					m_target(target)
				{}

			}
		}
	}
}