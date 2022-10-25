
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

				std::shared_ptr<GameObject> HidePlacePatrol::SearchTarget(const std::shared_ptr<I_FactionMember>& member) {
					//探しものがないなら、処理を終了。
					auto hidePlaces = ShareClassesManager::GetInstance()->GetCastShareClasses<HidePlace>();
					if (hidePlaces.size() == 0) {
						return nullptr;
					}

					//同じターゲット以外を探す。
					for (auto& weakHidePlace : hidePlaces) {
						auto hidePlace = weakHidePlace.lock();
						//nullptrまたは、すでにOpenなら対象にならない。
						if (hidePlace == nullptr || hidePlace->IsOpen()) {
							continue;
						}

						//他のメンバーがターゲットにしていなかったら
						if (!IsSomeMemberTarget(hidePlace->GetGameObject())) {
							return hidePlace->GetGameObject();	//そのターゲットを返す。
						}
					}

					return nullptr;
				}

				bool HidePlacePatrol::IsEnd() const {
					auto hidePlaces = ShareClassesManager::GetInstance()->GetShareClasses<HidePlace>();
					return hidePlaces.size() == 0;	//隠し場所が0なら処理が終了
				}

			}

		}
	}
}