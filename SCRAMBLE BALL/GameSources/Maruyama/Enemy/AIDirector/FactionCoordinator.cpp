
/*!
@file FactionCoordinator.cpp
@brief FactionCoordinatorのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "FactionCoordinator.h"

#include "Maruyama/Interface/I_FactionMember.h"
#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "EnemyAIDirector.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "PatrolCoordinator.h"
#include "CombatCoordinator.h"

#include "Maruyama/Utility/Component/TargetManager.h"
#include "Maruyama/Enemy/Component/Stator/StatorBase.h"
#include "Maruyama/Enemy/Component/Stator/EnemyStatorBase.h"

#include "PatrolCoordinator/HidePlacePatrol.h"
#include "Maruyama/Interface/I_TeamMember.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {

	namespace Enemy {

		FactionCoordinator::FactionCoordinator(const std::shared_ptr<AIDirector>& director):
			FactionCoordinator(director, std::vector<std::weak_ptr<EnemyBase>>())
		{}

		FactionCoordinator::FactionCoordinator(const std::shared_ptr<AIDirector>& director, const std::vector<std::weak_ptr<EnemyBase>>& members):
			CoordinatorBase(members), m_director(director)
		{}

		void FactionCoordinator::OnCreate() {
			//初めはパトロール設定をする。
			CreateFaction<AICoordinator::Patrol::HidePlacePatrol>(GetMembers());
		}

		void FactionCoordinator::OnStart() {
			//通知設定
			GetDirector()->GetTupleSpace()->Notify<Tuple::FindBall>(
				GetThis<FactionCoordinator>(),
				[&](const std::shared_ptr<Tuple::FindBall>& tuple) { NotifyTuple_FindBall(tuple); }
			);
		}

		bool FactionCoordinator::OnUpdate() {
			for (auto& coordinator : m_allCoordinators) {
				coordinator->OnUpdate();
			}

			return false;
		}

		void FactionCoordinator::OnExit() {
			//通知削除
			GetDirector()->GetTupleSpace()->RemoveAllNotifys(GetThis<FactionCoordinator>());
		}

		//ボールを見つけた通知を受け取る。
		void FactionCoordinator::NotifyTuple_FindBall(const std::shared_ptr<Tuple::FindBall>& tuple) {
			for (auto& weakMember : GetMembers()) {
				auto member = weakMember.lock();
				auto teamMember = member->GetSelfObject()->GetComponent<I_TeamMember>(false);
				if (!teamMember) {	//チームメンバーでなかったら
					continue;
				}

				//見つけた本人だったら、ゴールを目指す。
				if (member == tuple->GetRequester()) {
						//ゴールを目指す命令を出す。
					continue;	//その後の処理をしない。
				}

				//ボールを発見した相手が、自分と同じメンバーなら。
				if (teamMember->GetTeam() == tuple->GetTeamMember()->GetTeam()) {
						//ヘルプ行動や、集団行動を取る。
				}
				else {	//発見した相手が、違うメンバーなら。
						//ボールを持っている相手をターゲットにする。
				}
			}
		}

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------






		//--------------------------------------------------------------------------------------
		/// デバッグ
		//--------------------------------------------------------------------------------------

		void FactionCoordinator::DebugWriteTarget(const std::shared_ptr<I_FactionMember>& member, const std::shared_ptr<GameObject>& target) {
			auto patrol = member->GetAssignedFaction<PatrolCoordinator>();

			for (auto& weakMember : patrol->GetMembers()) {
				if (member == weakMember.lock()) {
					continue;
				}

				auto object = weakMember.lock()->GetGameObject();
				auto stator = object->GetComponent<I_Stator_EnumType<Enemy::StateType>>(false);
				auto targetManager = object->GetComponent<TargetManager>(false);

				if (stator && targetManager) {
					//仮で追いかけるステートなら更新しない。
					if (stator->IsCurrentState(Enemy::StateType::Chase)) {
						return;
					}

					stator->ChangeState(Enemy::StateType::Chase, (int)Enemy::StateType::Chase);
					targetManager->SetTarget(target);
				}
			}
		}
	}
}