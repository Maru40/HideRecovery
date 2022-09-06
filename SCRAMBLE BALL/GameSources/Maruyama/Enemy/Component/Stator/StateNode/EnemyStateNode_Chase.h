
/*!
@file EnemyStateNode_Chase.h
@brief EnemyStateNode_Chase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Enemy/StateMachine/EnemyStateNodeBase.h"

#include "Maruyama/Utility/Component/SeekTarget.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class TargetManager;
	template<class node_type, class EnumType, class TransitionType>
	class EnemyMainStateMachine;

	namespace Enemy {

		class EnemyBase;

		namespace StateNode {

			namespace ChaseState {
				struct TargetSeekParametor;
				struct AstarSeekParametor;
			}

			//--------------------------------------------------------------------------------------
			/// �X���C���̒Ǐ]�X�e�[�g
			//--------------------------------------------------------------------------------------
			class Chase : public EnemyStateNodeBase<EnemyBase>
			{
			public:
				/// <summary>
				/// �Ǐ]�X�e�[�g�̃X�e�[�g�^�C�v
				/// </summary>
				enum class StateType {
					TargetSeek,
					AstarSeek,
					Null,
				};

				/// <summary>
				/// �J�ڏ��������o�[
				/// </summary>
				struct TransitionMember
				{

				};

				/// <summary>
				/// �Ǐ]�p�����[�^
				/// </summary>
				struct Parametor
				{
					std::shared_ptr<ChaseState::TargetSeekParametor> targetSeekParamPtr = nullptr; //�^�[�Q�b�g�Ǐ]�p�����[�^
					std::shared_ptr<ChaseState::AstarSeekParametor> astarSeekParamPtr = nullptr;   //Astar���g�����Ǐ]�p�����[�^

					Parametor();
				};

				using StateMachine = EnemyMainStateMachine<EnemyBase, StateType, TransitionMember>;

			private:
				ex_weak_ptr<Parametor> m_paramPtr;            //�p�����[�^

				std::unique_ptr<StateMachine> m_stateMachine; //�X�e�[�g�}�V��

			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="objPtr">���̃N���X�����L����N���X</param>
				/// <param name="numCreate">�p�����[�^</param>
				Chase(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

				void OnStart() override;
				bool OnUpdate() override;
				void OnExit() override;

			private:
				/// <summary>
				/// �m�[�h�̐���
				/// </summary>
				void CreateNode();

				/// <summary>
				/// �G�b�W�̐���
				/// </summary>
				void CreateEdge();

				//--------------------------------------------------------------------------------------
				/// �J�ڏ���
				//--------------------------------------------------------------------------------------

				/// <summary>
				/// �Ǐ]�ړ�
				/// </summary>
				/// <param name="member">�J�ڃ����o</param>
				bool ToChaseMove(const TransitionMember& member);

				/// <summary>
				/// Astar���g�����Ǐ]�ړ�
				/// </summary>
				/// <param name="member">�J�ڃ����o</param>
				bool ToAstarMove(const TransitionMember& member);
				
			};

		}
	}
}