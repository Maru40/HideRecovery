
/*!
@file CombatCoordinator.cpp
@brief CombatCoordinator�̃N���X����
�S���F�ێR�T��
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

		CombatCoordinator::CombatCoordinator(const std::shared_ptr<FactionCoordinator>& owner)
			: HereOwnerCoordinatorBase(owner)
		{}

		bool CombatCoordinator::OnUpdate() {
			for (const auto& data : m_requestDatas) {
				CreateOrder(data);
			}

			ObserveFindTarget();

			return false;
		}

		void CombatCoordinator::CreateOrder(const Data& data) {

		}

		void CombatCoordinator::ObserveFindTarget() {
			using namespace basecross::Enemy;

			auto tupleSpace = GetTupleSpace();
			auto takes = tupleSpace->Takes<Tuple::FindTarget>();	//�^�[�Q�b�g�擾�̎҂��擾

			//if (!HasTarget()) {
			//	return;
			//}

			//auto faction = m_factionMember.lock()->GetAssignedFaction();
			//auto tupleSpace = faction->GetTupleSpace();

			//auto tuples = tupleSpace->Takes<Tuple::FindTarget>();

			//if (tuples.empty()) {	//�^�v�������݂��Ȃ��Ȃ珈�������Ȃ��B
			//	return;
			//}

			////�^�v���X�y�[�X�̕]���l�ɍ��킹���\�[�g
			//auto sortFunc = [](const std::shared_ptr<Tuple::FindTarget>& left, const std::shared_ptr<Tuple::FindTarget>& right) {
			//	return left->GetValue() < right->GetValue();
			//};
			//std::sort(tuples.begin(), tuples.end(), sortFunc);

			//for (int i = 0; i < tuples.size(); i++) {
			//	auto& tuple = tuples[i];

			//	auto toTargetRange = m_targetManager.lock()->CalcuToTargetVec().length();
			//	if (tuple->GetValue() < toTargetRange) {	//�V�K�������݂̕����]���l������������
			//		m_targetManager.lock()->SetTarget(tuple->GetTarget());
			//	}

			//	RemoveTuples(tuples, tuple->GetRequester());
			//}
		}

		void CombatCoordinator::Request(const std::shared_ptr<EnemyBase>& member, const Data& data) {
			m_requestDatas.push_back(data);
		}

	}
}