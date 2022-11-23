
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
			//�ʒm�ݒ�

			//�L��
			GetTupleSpace()->Notify<Tuple::Kill>(
				GetThis<CombatCoordinator>(),
				[&](const std::shared_ptr<Tuple::Kill>& tuple) { NotifyTuple_Kill(tuple); }
			);

			//�^�[�Q�b�g�T�[�`
			GetTupleSpace()->Notify<Tuple::SearchTarget>(
				GetThis<CombatCoordinator>(),
				[&](const std::shared_ptr<Tuple::SearchTarget>& tuple) { NotifyTuple_SearchTarget(tuple); }
			);

			//�^�[�Q�b�g����
			GetTupleSpace()->Notify<Tuple::FindTarget>(
				GetThis<CombatCoordinator>(),
				[&](const std::shared_ptr<Tuple::FindTarget>& tuple) { NotifyTuple_FindTarget(tuple); }
			);

			//�^�[�Q�b�g�����������Ƃ��̒ʒm
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
			//�ʒm�̍폜
			GetTupleSpace()->RemoveAllNotifys(GetThis<CombatCoordinator>());
		}

		void CombatCoordinator::UpdateObserveFindTarget() {
			auto tupleSpace = GetTupleSpace();
			auto takes = tupleSpace->Takes<Tuple::FindTarget>();	//�^�[�Q�b�g�擾�̎҂��擾
		}

		void CombatCoordinator::UpdateObserveDamaged() {
			auto takes = GetTupleSpace()->Takes<Tuple::Damaged>();
		}

		void CombatCoordinator::NotifyTuple_Kill(const std::shared_ptr<Tuple::Kill>& tuple) {
			//�L�������^�[�Q�b�g�̎擾
			RemoveTaret(tuple->GetKilled());

			GetTupleSpace()->Take(tuple);
		}

		void CombatCoordinator::NotifyTuple_SearchTarget(const std::shared_ptr<Tuple::SearchTarget>& tuple) {
			GetTupleSpace()->Take(tuple);

			//�^�[�Q�b�g�̐���


			if (m_targets.empty()) {	//�^�[�Q�b�g�����݂��Ȃ��Ȃ�B
				//�p�g���[���ɕύX�B
				tuple->GetRequester()->GetTupleSpace()->Write<Tuple::PatrolTransition>(
					GetThis<CombatCoordinator>(),
					0.0f
				);	
				return;
			}

			//�^�[�Q�b�g�̕ύX
			auto target = SearchPriorityTarget(tuple->GetTargetManager()->GetGameObject());
			tuple->GetTargetManager()->SetTarget(target);
		}

		std::shared_ptr<GameObject> CombatCoordinator::SearchPriorityTarget(const std::shared_ptr<GameObject>& requester) {
			if (m_targets.empty()) {
				return nullptr;	//�o�^����Ă���^�[�Q�b�g�����݂��Ȃ��Ȃ�Anullptr��Ԃ��B
			}

			//�\�[�g�p�̉��f�[�^�\����
			struct Data {
				std::weak_ptr<GameObject> target;	//�ڕW
				float evalutionValue;				//�]���l

				Data(const std::shared_ptr<GameObject>& target, const float value):
					target(target), evalutionValue(value)
				{}
			};

			std::vector<Data> datas; //�]���lData�z��

			for (auto& target : m_targets) {
				//�^�[�Q�b�g���^�[�Q�b�g�w��ł��Ȃ��Ȃ�
				auto targeted = target.lock()->GetComponent<Targeted>();
				if (targeted && !targeted->CanTarget()) {
					continue;	//�^�[�Q�b�g����Ȃ��B
				}

				float value = Calculater::Combat::CalculateEvalutionValue(requester, target.lock());
				datas.push_back(Data(target.lock(), value));
			}

			//�f�[�^�����݂��Ȃ��Ȃ�nullptr��Ԃ��B
			if (datas.empty()) {
				return nullptr;
			}

			//�\�[�g
			auto sortFunc = [](const Data& left, const Data& right) {	
				return left.evalutionValue < right.evalutionValue;	//�\�[�g����
			};
			std::sort(datas.begin(), datas.end(), sortFunc);

			return datas[0].target.lock();	//��ԕ]������Ă���^�[�Q�b�g���擾
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
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		void CombatCoordinator::AddTarget(const std::shared_ptr<GameObject>& target) {
			//�o�^����Ă��Ȃ�������A�ǉ�����B
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