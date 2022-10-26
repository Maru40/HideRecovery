
/*!
@file PatrolCoordinator.cpp
@brief PatrolCoordinatorのクラス実体
担当：丸山裕喜
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

				std::vector<std::shared_ptr<GameObject>> HidePlacePatrol::SearchOtherTargets(
					const std::shared_ptr<I_FactionMember>& member,
					const std::vector<std::weak_ptr<HidePlace>>& hidePlaces
				) {
					std::vector<std::shared_ptr<GameObject>> result;

					for (auto& weakHidePlace : hidePlaces) {
						auto hidePlace = weakHidePlace.lock();
						//nullptrまたは、すでにOpenなら対象にならない。
						if (hidePlace == nullptr || hidePlace->IsOpen()) {
							continue;
						}

						//他のメンバーがターゲットにしていなかったら
						if (!IsSomeMemberTarget(hidePlace->GetGameObject())) {
							result.push_back(hidePlace->GetGameObject());	//そのターゲットを返す。
						}
					}

					return result;
				}

				std::shared_ptr<GameObject> HidePlacePatrol::SearchTarget(const std::shared_ptr<I_FactionMember>& member) {
					//探しものがないなら、処理を終了。
					auto hidePlaces = ShareClassesManager::GetInstance()->GetCastShareClasses<HidePlace>();
					if (hidePlaces.size() == 0) {
						return nullptr;
					}

					//他のメンバーがターゲットにしていない者のみに絞る。
					auto otherTargets = SearchOtherTargets(member, hidePlaces);

					//ターゲット候補が0なら、nullptrを返す(ターゲットが存在しない。)
					if (otherTargets.empty()) {
						return nullptr;
					}

					//ターゲット候補の中から一番近いターゲットが手前に来るようにソートする。
					auto sortFunction = [&](const std::shared_ptr<GameObject>& left, const std::shared_ptr<GameObject>& right) {
						auto memberPosition = member->GetSelfObject()->GetComponent<Transform>()->GetPosition();
						auto toLeftRange = (left->GetComponent<Transform>()->GetPosition() - memberPosition).length();

						auto toRightRange = (right->GetComponent<Transform>()->GetPosition() - memberPosition).length();

						return toLeftRange < toRightRange;
					};
					std::sort(otherTargets.begin(), otherTargets.end(), sortFunction);

					return otherTargets[0];
				}

				bool HidePlacePatrol::IsEnd() const {
					auto hidePlaces = ShareClassesManager::GetInstance()->GetShareClasses<HidePlace>();
					return hidePlaces.size() == 0;	//隠し場所が0なら処理が終了
				}

			}

		}
	}
}