
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

#include "Maruyama/Utility/SingletonComponent/ShareClassesManager.h"

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
				{}

				void HidePlacePatrol::OnStart() {
					
				}

				bool HidePlacePatrol::OnUpdate() {
					return IsEnd();
				}

				std::shared_ptr<GameObject> HidePlacePatrol::SearchTarget(const std::shared_ptr<I_FactionMember>& member) {
					//�T�����̂��Ȃ��Ȃ�A�������I���B
					auto hidePlaces = ShareClassesManager::GetInstance()->GetShareClasses<HidePlace>();
					if (hidePlaces.size() == 0) {
						return nullptr;
					}

					//�����o�[�̎擾
					for (auto& member : GetMembers()) {
						member.lock()->GetTarget();
					}
				}

				bool HidePlacePatrol::IsEnd() const {
					auto hidePlaces = ShareClassesManager::GetInstance()->GetShareClasses<HidePlace>();
					return hidePlaces.size() == 0;	//�B���ꏊ��0�Ȃ珈�����I��
				}

			}

		}
	}
}