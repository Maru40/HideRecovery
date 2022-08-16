
/*!
@file PatrolCoordinator.cpp
@brief PatrolCoordinatorのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "PatrolCoordinator.h"

#include "EnemyBase.h"
#include "FactionCoordinator.h"

#include "MaruUtility.h"

#include "Watanabe/DebugClass/Debug.h"

#include "I_FactionMember.h"

#include "EnemyMainStateMachine.h"
#include "StatorBase.h"
#include "EnemyStatorBase.h"

#include "TargetManager.h"

namespace basecross {

	namespace Enemy {

		PatrolCoordinator::PatrolCoordinator(const std::shared_ptr<FactionCoordinator>& owner):
			PatrolCoordinator(owner, std::vector<std::weak_ptr<EnemyBase>>())
		{}

		PatrolCoordinator::PatrolCoordinator(const std::shared_ptr<FactionCoordinator>& owner, const std::vector<std::weak_ptr<EnemyBase>>& members):
			HereOwnerCoordinatorBase(owner, members)
		{}

		void PatrolCoordinator::OnStart() {
			//メンバーにグループ調整者やタイプを伝える。
			for (auto& member : GetMembers()) {
				if (auto iFactionMember = member.lock()->GetGameObject()->GetComponent<I_FactionMember>(false)) {
					//将来的にデータでまとめる。
					iFactionMember->SetFactionType(FactionType::Patrol);
					iFactionMember->SetFactionCoordinator(GetOwner());
					iFactionMember->SetAssignFaction(GetThis<PatrolCoordinator>());
				}
			}

			//メンバーの目的地をそれぞれ変更

		}

		void PatrolCoordinator::OnUpdate() {
			PatrolControl();

			ObserveTuple_FindTarget();
		}

		void PatrolCoordinator::PatrolControl() {
			
		}

		void PatrolCoordinator::ObserveTuple_FindTarget() {
			auto tupleSpace = GetTupleSpace();
			auto tuple = tupleSpace->Take<Tuple::FindTarget>();
			if (!tuple) {
				return;
			}

			auto target = tuple->target.lock();

			for (auto& weakMember : GetMembers()) {
				auto member = weakMember.lock();

				auto stator = member->GetGameObject()->GetComponent<I_Stator_EnumType<Enemy::StateType>>(false);
				auto targetManager = member->GetGameObject()->GetComponent<TargetManager>(false);
				if (!stator || !targetManager) {
					continue;
				}

				if (stator->IsCurrentState(Enemy::StateType::Chase)) {	//ステートが同じなら処理しない
					continue;
				}

				targetManager->SetTarget(target);
				stator->ChangeState(Enemy::StateType::Chase, (int)Enemy::StateType::Chase);
			}
		}
	}
}