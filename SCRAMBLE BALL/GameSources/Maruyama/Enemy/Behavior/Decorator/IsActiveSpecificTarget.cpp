/*!
@file IsActiveSpecificTarget.cpp
@brief IsActiveSpecificTarget‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

#include "IsActiveSpecificTarget.h"

namespace basecross {
	namespace maru {

		namespace Behavior {
			namespace Decorator {

				IsAcitiveSpecificTarget::IsAcitiveSpecificTarget(
					const std::shared_ptr<GameObject>& owner,
					const std::shared_ptr<GameObject>& target
				):
					DecoratorBase(owner),
					m_target(target)
				{}

				bool IsAcitiveSpecificTarget::CanTransition() const {
					return m_target.lock()->IsActive();
				}

				bool IsAcitiveSpecificTarget::CanUpdate() {
					return CanTransition();
				}

			}
		}
	}
}