
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

namespace basecross {

	namespace Enemy {

		using namespace basecross::Enemy;

		CombatCoordinator::CombatCoordinator(const std::shared_ptr<FactionCoordinator>& owner)
			: HereOwnerCoordinatorBase(owner)
		{
			
		}

		void CombatCoordinator::OnCreate() {
			
		}

		bool CombatCoordinator::OnUpdate() {
			UpdateObserveFindTarget();
			UpdateObserveDamaged();

			return false;
		}

		void CombatCoordinator::UpdateObserveFindTarget() {
			auto tupleSpace = GetTupleSpace();
			auto takes = tupleSpace->Takes<Tuple::FindTarget>();	//ターゲット取得の者を取得
		}

		void CombatCoordinator::UpdateObserveDamaged() {
			auto takes = GetTupleSpace()->Takes<Tuple::Damaged>();

			//自分のターゲットと違うならHelp

			//Helpが来るなら→HPが低い方を攻撃

			//Helpが来ないなら→逃げる。
		}

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		void CombatCoordinator::AddTargets(const std::shared_ptr<GameObject>& target) {
			//登録されていなかったら、追加する。
			if (!HasTarget(target)) {
				m_targets.push_back(target);
			}
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