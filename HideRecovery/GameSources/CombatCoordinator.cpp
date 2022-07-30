
/*!
@file CombatCoordinator.cpp
@brief CombatCoordinatorのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "CombatCoordinator.h"

#include "EnemyBase.h"
#include "FactionCoordinator.h"

#include "MaruUtility.h"

namespace basecross {

	namespace Enemy {

		CombatCoordinator::CombatCoordinator(const std::shared_ptr<FactionCoordinator>& owner)
			: CombatCoordinator(owner, std::vector<ex_weak_ptr<EnemyBase>>())
		{}
		CombatCoordinator::CombatCoordinator(const std::shared_ptr<FactionCoordinator>& owner, const std::shared_ptr<EnemyBase>& member)
			: CombatCoordinator(owner, std::vector<ex_weak_ptr<EnemyBase>>(1, member))
		{}
		CombatCoordinator::CombatCoordinator(const std::shared_ptr<FactionCoordinator>& owner, const vector<ex_weak_ptr<EnemyBase>>& members)
			: m_owner(owner), m_members(members)
		{}

		void CombatCoordinator::OnUpdate() {
			for (const auto& data : m_requestDatas) {
				CreateOrder(data);
			}
		}

		void CombatCoordinator::CreateOrder(const Data& data) {
		}

		void CombatCoordinator::Assign(const std::shared_ptr<EnemyBase>& member) {
			m_members.push_back(member);
		}

		void CombatCoordinator::UnAssign(const std::shared_ptr<EnemyBase>& member) {
			ex_weak_ptr<EnemyBase> temp = member;
			maru::Utility::RemoveVec(m_members, temp);
		}

		void CombatCoordinator::Request(const std::shared_ptr<EnemyBase>& member, const Data& data) {
			m_requestDatas.push_back(data);
		}

		std::shared_ptr<FactionCoordinator> CombatCoordinator::GetOwner() const {
			return m_owner.GetShard();
		}

	}
}