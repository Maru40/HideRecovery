
/*!
@file StateNode_HidePlacePatrol.h
@brief StateNode_HidePlacePatrol�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/StateMachine/Node_StateMachine.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class ObserveIsInEyeTarget;
	class I_TeamMember;
	class EyeSearchRange;
	class TargetManager;

	namespace maru {
		namespace Behavior {
			namespace SubBehavior {

				class HidePlacePatrolTree;

			}
		}
	}

	namespace Enemy {

		class EnemyBase;
		class I_FactionMember;

		namespace Tuple {
			class Damaged;
		}

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			///	�B���ꏊ��T���X�e�[�g
			//--------------------------------------------------------------------------------------
			class HidePlacePatrol : public maru::NodeBase_StateMachine<EnemyBase>
			{
			public:
				using HidePlacePatrolTree = maru::Behavior::SubBehavior::HidePlacePatrolTree;

			private:
				std::unique_ptr<HidePlacePatrolTree> m_behaviorTree;			//�r�w�C�r�A�c���[

				std::unique_ptr<ObserveIsInEyeTarget> m_observeButtleTarget;	//�o�g���p�ɊĎ��������Ώ�

				std::weak_ptr<I_TeamMember> m_teamMember;			//�`�[�������o�[
				std::weak_ptr<I_FactionMember> m_factionMember;		//�t�@�N�V���������o�[
				std::weak_ptr<TargetManager> m_targetManager;

			public:
				HidePlacePatrol(const std::shared_ptr<EnemyBase>& owner);

				virtual ~HidePlacePatrol() = default;

				void OnStart() override;
				bool OnUpdate() override;
				void OnExit() override;

			private:

				/// <summary>
				/// �_���[�W���󂯂����̏���
				/// </summary>
				void Damaged(const std::shared_ptr<Tuple::Damaged>& tuple);

				/// <summary>
				/// �o�g���p�̃^�[�Q�b�g���Ď�����B
				/// </summary>
				void ObserveButtleTarget();

				/// <summary>
				/// �B���ꏊ��T���Ă���Ƃ��̃^�[�Q�b�g��ݒ肷��B
				/// </summary>
				void SettingObserveButtleTargets();
				
			};

		}
	}
}