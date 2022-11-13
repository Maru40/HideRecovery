/*!
@file OutSpecificTarget.cpp
@brief OutSpecificTarget‚È‚ÇÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

#include "RegisterCanTransitionDecorator.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Decorator {

				bool ResgisterCanTrasitionDecorator::CanTransition() const {
					for (auto& canTransition : m_canTransitions) {
						//ˆê‚Â‚Å‚à‘JˆÚ‚Å‚«‚È‚¢ğŒ‚ª‚ ‚Á‚½‚çB
						if (!canTransition()) {	
							return false;	//‘JˆÚ•s‰Â
						}
					}

					return true;
				}

				bool ResgisterCanTrasitionDecorator::CanUpdate() {
					return CanTransition();
				}

				void ResgisterCanTrasitionDecorator::AddCanTransitionFunction(const std::function<bool()>& canTransition) {
					m_canTransitions.push_back(canTransition);
				}

			}
		}
	}
}