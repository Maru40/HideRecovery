
/*!
@file PatrolCoordinator.cpp
@brief PatrolCoordinator�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "HidePlacePatrol.h"

#include "../PatrolCoordinator.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "../FactionCoordinator.h"

#include "Maruyama/StageObject/HidePlace.h"

#include "Maruyama/Utility/Utility.h"

namespace basecross {

	namespace Enemy {

		namespace AICoordinator {

			namespace Patrol {

				HidePlacePatrol::HidePlacePatrol(const std::shared_ptr<FactionCoordinator>& owner) :
					HidePlacePatrol(owner, std::vector<std::weak_ptr<EnemyBase>>())
				{}

				HidePlacePatrol::HidePlacePatrol(const std::shared_ptr<FactionCoordinator>& owner, const std::vector<std::weak_ptr<EnemyBase>>& members) :
					PatrolCoordinator(owner, members),
					m_param(Parametor())
				{
					m_hidePlaces = maru::Utility::FindWeakPtrComponents<HidePlace>();
				}

				void HidePlacePatrol::OnStart() {
					
				}

				bool HidePlacePatrol::OnUpdate() {
					//�T�����̂��Ȃ��Ȃ�A�����B
					if (m_hidePlaces.size() == 0) {
						m_hidePlaces = maru::Utility::FindWeakPtrComponents<HidePlace>();
					}



					return false;
				}

			}

		}
	}
}