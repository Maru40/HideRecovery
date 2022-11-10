
/*!
@file StateNode_HidePlacePatrol.h
@brief StateNode_HidePlacePatrol�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Enemy/StateMachine/EnemyStateNodeBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class TargetManager;

	namespace maru {
		namespace Behavior {
			namespace SubBehavior {

				//class HidePlacePatrolTree;
				class ButtleTree;

			}
		}
	}

	namespace Enemy {

		class EnemyBase;
		class I_FactionMember;

		namespace Tuple {
			class FindTarget;
			class Damaged;
			class Kill;
		}

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			///	�o�g���X�e�[�g
			//--------------------------------------------------------------------------------------
			class Buttle : public EnemyStateNodeBase<EnemyBase>
			{
			public:
				using ButtleTree = maru::Behavior::SubBehavior::ButtleTree;

			private:
				std::weak_ptr<Transform> m_transform;
				std::weak_ptr<TargetManager> m_targetManager;

				std::unique_ptr<ButtleTree> m_behaviorTree;

			public:
				Buttle(const std::shared_ptr<EnemyBase>& owner);

				void OnStart() override;

				bool OnUpdate() override;

				void OnExit() override;

			private:

				/// <summary>
				/// �ʒm�ݒ�
				/// </summary>
				void SettingNotify();

				/// <summary>
				/// �^�[�Q�b�g�������b�Z�[�W���Ď�����B
				/// </summary>
				/// <param name="tuple"></param>
				void ObserveOtherFindTarget(const std::shared_ptr<Tuple::FindTarget>& tuple);

				/// <summary>
				/// �_���[�W���b�Z�[�W���Ď�����B
				/// </summary>
				/// <param name="tuple">�_���[�W���b�Z�[�W</param>
				void ObserveDamaged(const std::shared_ptr<Tuple::Damaged>& tuple);

				/// <summary>
				/// �������g�ւ̃_���[�W����
				/// </summary>
				/// <param name="tuple">�_���[�W���b�Z�[�W</param>
				void SelfDamaged(const std::shared_ptr<Tuple::Damaged>& tuple);

				/// <summary>
				/// �����ȊO���_���[�W���󂯂���
				/// </summary>
				/// <param name="tuple">�_���[�W���b�Z�[�W</param>
				void OtherDamaged(const std::shared_ptr<Tuple::Damaged>& tuple);

				/// <summary>
				/// �L�����b�Z�[�W���󂯎�����Ƃ��B
				/// </summary>
				/// <param name="tuple"></param>
				void NotifyTuple_Kill(const std::shared_ptr<Tuple::Kill>& tuple);

				//���̃^�[�Q�b�g��_���]���l���v�Z���ĕԂ��B
				float CalculateEvalutionValue(const std::shared_ptr<GameObject>& target);

				bool HasTarget() const;

			};

		}
	}
}