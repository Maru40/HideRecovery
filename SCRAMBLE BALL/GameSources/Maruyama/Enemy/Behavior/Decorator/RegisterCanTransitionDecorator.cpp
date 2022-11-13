/*!
@file OutSpecificTarget.cpp
@brief OutSpecificTarget�Ȃǎ���
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
						//��ł��J�ڂł��Ȃ���������������B
						if (!canTransition()) {	
							return false;	//�J�ڕs��
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