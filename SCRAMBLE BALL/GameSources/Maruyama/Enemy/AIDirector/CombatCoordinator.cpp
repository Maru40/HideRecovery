
/*!
@file CombatCoordinator.cpp
@brief CombatCoordinatorのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "CombatCoordinator.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "FactionCoordinator.h"
#include "Maruyama/Enemy/AIDirector/TupleSpace.h"

#include "Maruyama/Utility/Utility.h"
#include "Maruyama/Enemy/AIDirector/Calculater/CombatCalculater.h"

#include "Maruyama/Utility/Component/TargetManager.h"
#include "Maruyama/Utility/Component/Targeted.h"

namespace basecross {

	namespace Enemy {

		using namespace basecross::Enemy;

		CombatCoordinator::CombatCoordinator(const std::shared_ptr<FactionCoordinator>& owner)
			: HereOwnerCoordinatorBase(owner)
		{
			
		}

		void CombatCoordinator::OnCreate() {
			
		}

		void CombatCoordinator::OnStart() {
			//通知設定

			//キル
			GetTupleSpace()->Notify<Tuple::Kill>(
				GetThis<CombatCoordinator>(),
				[&](const std::shared_ptr<Tuple::Kill>& tuple) { NotifyTuple_Kill(tuple); }
			);

			//ターゲットサーチ
			GetTupleSpace()->Notify<Tuple::SearchTarget>(
				GetThis<CombatCoordinator>(),
				[&](const std::shared_ptr<Tuple::SearchTarget>& tuple) { NotifyTuple_SearchTarget(tuple); }
			);

			//ターゲット発見
			GetTupleSpace()->Notify<Tuple::FindTarget>(
				GetThis<CombatCoordinator>(),
				[&](const std::shared_ptr<Tuple::FindTarget>& tuple) { NotifyTuple_FindTarget(tuple); }
			);

			//ターゲットを見失ったときの通知
			GetTupleSpace()->Notify<Tuple::LostTarget>(
				GetThis<CombatCoordinator>(),
				[&](const std::shared_ptr<Tuple::LostTarget>& tuple) { NotifyTuple_LostTarget(tuple); }
			);
		}

		bool CombatCoordinator::OnUpdate() {
			UpdateObserveFindTarget();
			UpdateObserveDamaged();

			return false;
		}

		void CombatCoordinator::OnExit() {
			//通知の削除
			GetTupleSpace()->RemoveAllNotifys(GetThis<CombatCoordinator>());
		}

		void CombatCoordinator::UpdateObserveFindTarget() {
			auto tupleSpace = GetTupleSpace();
			auto takes = tupleSpace->Takes<Tuple::FindTarget>();	//ターゲット取得の者を取得
		}

		void CombatCoordinator::UpdateObserveDamaged() {
			auto takes = GetTupleSpace()->Takes<Tuple::Damaged>();
		}

		void CombatCoordinator::NotifyTuple_Kill(const std::shared_ptr<Tuple::Kill>& tuple) {
			//キルしたターゲットの取得
			RemoveTaret(tuple->GetKilled());

			GetTupleSpace()->Take(tuple);
		}

		void CombatCoordinator::NotifyTuple_SearchTarget(const std::shared_ptr<Tuple::SearchTarget>& tuple) {
			GetTupleSpace()->Take(tuple);

			//ターゲットの整理


			if (m_targets.empty()) {	//ターゲットが存在しないなら。
				//パトロールに変更。
				tuple->GetRequester()->GetTupleSpace()->Write<Tuple::PatrolTransition>(
					GetThis<CombatCoordinator>(),
					0.0f
				);	
				return;
			}

			//ターゲットの変更
			auto target = SearchPriorityTarget(tuple->GetTargetManager()->GetGameObject());
			tuple->GetTargetManager()->SetTarget(target);
		}

		std::shared_ptr<GameObject> CombatCoordinator::SearchPriorityTarget(const std::shared_ptr<GameObject>& requester) {
			if (m_targets.empty()) {
				return nullptr;	//登録されているターゲットが存在しないなら、nullptrを返す。
			}

			//ソート用の仮データ構造体
			struct Data {
				std::weak_ptr<GameObject> target;	//目標
				float evalutionValue;				//評価値

				Data(const std::shared_ptr<GameObject>& target, const float value):
					target(target), evalutionValue(value)
				{}
			};

			std::vector<Data> datas; //評価値Data配列

			for (auto& target : m_targets) {
				//ターゲットがターゲット指定できないなら
				auto targeted = target.lock()->GetComponent<Targeted>();
				if (targeted && !targeted->CanTarget()) {
					continue;	//ターゲットから省く。
				}

				float value = Calculater::Combat::CalculateEvalutionValue(requester, target.lock());
				datas.push_back(Data(target.lock(), value));
			}

			//データが存在しないならnullptrを返す。
			if (datas.empty()) {
				return nullptr;
			}

			//ソート
			auto sortFunc = [](const Data& left, const Data& right) {	
				return left.evalutionValue < right.evalutionValue;	//ソート条件
			};
			std::sort(datas.begin(), datas.end(), sortFunc);

			return datas[0].target.lock();	//一番評価されているターゲットを取得
		}

		void CombatCoordinator::NotifyTuple_FindTarget(const std::shared_ptr<Tuple::FindTarget>& tuple) {
			GetTupleSpace()->Take(tuple);

			AddTarget(tuple->GetTarget());
		}

		void CombatCoordinator::NotifyTuple_LostTarget(const std::shared_ptr<Tuple::LostTarget>& tuple) {
			GetTupleSpace()->Take(tuple);

			RemoveTaret(tuple->GetTargetManager()->GetTarget());
		}

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		void CombatCoordinator::AddTarget(const std::shared_ptr<GameObject>& target) {
			//登録されていなかったら、追加する。
			if (!HasTarget(target)) {
				m_targets.push_back(target);
			}
		}

		bool CombatCoordinator::RemoveTaret(const std::shared_ptr<GameObject>& removeTaret) {
			auto iter = m_targets.begin();
			while (iter != m_targets.end()) {
				if ((*iter).lock() == removeTaret) {
					iter = m_targets.erase(iter);
					return true;
				}
				iter++;
			}

			return false;
		}

		bool CombatCoordinator::HasTarget(const std::shared_ptr<GameObject>& target) const {
			for (const auto& weakTarget : m_targets) {
				if (weakTarget.lock() == target) {
					return true;
				}
			}
			
			return false;
		}

	}
}