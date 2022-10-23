
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
					PatrolCoordinator::OnStart();


				}

				bool HidePlacePatrol::OnUpdate() {
					return IsEnd();
				}

				void HidePlacePatrol::OnExit() {

				}

				bool HidePlacePatrol::IsSomeMemberTarget(const std::shared_ptr<GameObject>& target) {
					for (auto& weakMember : GetMembers()) {
						auto member = weakMember.lock();
						if (member->GetTarget() == target) {
							return true;
						}
					}

					return false;
				}

				std::shared_ptr<GameObject> HidePlacePatrol::SearchTarget(const std::shared_ptr<I_FactionMember>& member) {
					//�T�����̂��Ȃ��Ȃ�A�������I���B
					auto hidePlaces = ShareClassesManager::GetInstance()->GetCastShareClasses<HidePlace>();
					if (hidePlaces.size() == 0) {
						return nullptr;
					}

					//�����^�[�Q�b�g�ȊO��T���B
					for (auto& weakHidePlace : hidePlaces) {
						auto hidePlace = weakHidePlace.lock();
						//nullptr�܂��́A���ł�Open�Ȃ�ΏۂɂȂ�Ȃ��B
						if (hidePlace == nullptr || hidePlace->IsOpen()) {
							continue;
						}

						//���̃����o�[���^�[�Q�b�g�ɂ��Ă��Ȃ�������
						if (!IsSomeMemberTarget(hidePlace->GetGameObject())) {
							return hidePlace->GetGameObject();	//���̃^�[�Q�b�g��Ԃ��B
						}
					}

					return nullptr;
				}

				bool HidePlacePatrol::IsEnd() const {
					auto hidePlaces = ShareClassesManager::GetInstance()->GetShareClasses<HidePlace>();
					return hidePlaces.size() == 0;	//�B���ꏊ��0�Ȃ珈�����I��
				}

			}

		}
	}
}