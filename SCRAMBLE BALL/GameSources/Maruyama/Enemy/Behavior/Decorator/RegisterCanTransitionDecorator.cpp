/*!
@file OutSpecificTarget.cpp
@brief OutSpecificTargetなど実体
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
						//一つでも遷移できない条件があったら。
						if (!canTransition()) {	
							return false;	//遷移不可
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