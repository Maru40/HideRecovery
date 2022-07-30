
/*!
@file Stator_BigSlime.h
@brief Stator_BigSlime�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Trigger.h"
#include "I_BrackBoard.h"

namespace basecross {
	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class EnemyBase;

		namespace StateNode {
			struct Slime_Dyning_Parametor;
			struct BigSlime_Dyning_Parametor;
		}

		namespace BigSlime {

			//--------------------------------------------------------------------------------------
			/// ����X���C���X�e�[�^�[�̑J�ڏ��������o�[
			//--------------------------------------------------------------------------------------
			struct Stator_TransitionMemeber {
				Trigger chaseTrigger;      //�Ǐ]�ɐ؂�ւ��g���K�[
				Trigger knockBackTrigger;  //�m�b�N�o�b�N�g���K�[
				float startChaseRange;     //�Ǐ]���n�߂鋗��
				float maxChaseRange;       //�ǐՂ���ő勗��
				float bodyBlowRange;       //�̓�������n�߂鋗��

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				Stator_TransitionMemeber();

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="startChaseRange">�Ǐ]���n�߂鋗��</param>
				/// <param name="maxChaseRange">�Ǐ]������ő勗��</param>
				/// <param name="bodyBlowRange">�̓�������n�߂鋗��</param>
				Stator_TransitionMemeber(const float& startChaseRange, const float& maxChaseRange, const float& bodyBlowRange);
			};

			//--------------------------------------------------------------------------------------
			/// ����X���C���X�e�[�^�[�̃u���b�N�{�[�h
			//--------------------------------------------------------------------------------------
			struct Stator_BlackBoard {
				std::shared_ptr<StateNode::Slime_Dyning_Parametor> dyningParamPtr;  //���S
			};

			//--------------------------------------------------------------------------------------
			/// ����X���C���X�e�[�^�[
			//--------------------------------------------------------------------------------------
			class Stator : public SlimeStatorBase<EnemyBase, Stator_TransitionMemeber>, public I_BrackBoard<Stator_BlackBoard>
			{
			public:
				using TransitionMember = Stator_TransitionMemeber;
				using BlackBoard = Stator_BlackBoard;

				struct Parametor;

			private:
				std::unique_ptr<Parametor> m_paramPtr = nullptr; //�p�����[�^

			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="owner">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
				Stator(const std::shared_ptr<GameObject>& owner);

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="owner">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
				/// <param name="member">�J�ڏ��������o�[</param>
				Stator(const std::shared_ptr<GameObject>& owner, const TransitionMember& member);

			private:
				void CreateNode() override;
				void CreateEdge() override;

				//--------------------------------------------------------------------------------------
				/// �J�ڏ���
				//--------------------------------------------------------------------------------------

				/// <summary>
				/// �{�f�B�u���[�����鋗��
				/// </summary>
				/// <param name="member">�J�ڏ��������o�[</param>
				/// <returns>�{�f�B�u���[�����鋗���Ȃ�true</returns>
				bool Stator::IsBodyBlowRange(const Stator::TransitionMember& member);

				//--------------------------------------------------------------------------------------
				/// �p�����[�^�ݒ�
				//--------------------------------------------------------------------------------------

				/// <summary>
				/// �p�����[�^�̐ݒ�
				/// </summary>
				void SettingParametor();

				/// <summary>
				/// �p�j�p�����[�^�̐ݒ�
				/// </summary>
				void SettingPlowling();

				/// <summary>
				/// �Ǐ]�p�����[�^�̐ݒ�
				/// </summary>
				void SettingChase();

			};

		}
	}
}