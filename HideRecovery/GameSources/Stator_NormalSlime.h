
/*!
@file Stator_NormalSlime.h
@brief Stator_NormalSlime�Ȃ�
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
		template<class NodeType, class EnumType, class TransitionMember>
		class StatorBase;

		namespace StateNode {
			struct Slime_Dyning_Parametor;
			struct Slime_Attack_Parametor;
		}

		//--------------------------------------------------------------------------------------
		///	�ʏ�X���C���̃X�e�[�^�[�̑J�ڏ��������o�[
		//--------------------------------------------------------------------------------------
		struct Stator_NormalSlime_TransitionMember
		{
			Trigger chaseTrigger;      //�Ǐ]�ɐ؂�ւ��g���K�[
			Trigger knockBackTrigger;  //�m�b�N�o�b�N�g���K�[
			float startChaseRange;     //�Ǐ]���n�߂鋗��
			float maxChaseRange;       //�ǐՂ���ő勗��
			float bodyBlowRange;       //�̓�������n�߂鋗��

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			Stator_NormalSlime_TransitionMember();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="startChaseRange">�Ǐ]���n�߂鋗��</param>
			/// <param name="maxChaseRange">�Ǐ]����ő勗��</param>
			/// <param name="bodyBlowRange">�̓�������n�߂鋗��</param>
			Stator_NormalSlime_TransitionMember(const float& startChaseRange, const float& maxChaseRange, const float& bodyBlowRange);
		};

		//--------------------------------------------------------------------------------------
		/// �ʏ�X���C���̃X�e�[�^�[�̃u���b�N�{�[�h
		//--------------------------------------------------------------------------------------
		struct Stator_NormalSlime_BlackBoard
		{
			std::shared_ptr<StateNode::Slime_Dyning_Parametor> deathParamPtr;  //���S
			std::shared_ptr<StateNode::Slime_Attack_Parametor> attackParamPtr; //�U��
		};

		//--------------------------------------------------------------------------------------
		/// �ʏ�X���C���̃X�e�[�^�[
		//--------------------------------------------------------------------------------------
		class Stator_NormalSlime : public SlimeStatorBase<EnemyBase, Stator_NormalSlime_TransitionMember>, public I_BrackBoard<Stator_NormalSlime_BlackBoard>
		{
		public:
			struct Parametor;

			using TransitionMember = Stator_NormalSlime_TransitionMember;
			using BlackBoard = Stator_NormalSlime_BlackBoard;

		private:
			std::unique_ptr<Parametor> m_paramPtr;  //�p�����[�^

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			Stator_NormalSlime(const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			/// <param name="member">�J�ڏ��������o�[</param>
			Stator_NormalSlime(const std::shared_ptr<GameObject>& objPtr, const TransitionMember& member);

		private:
			void CreateNode() override;
			void CreateEdge() override;

			/// <summary>
			/// �{�f�B�u���[�ɑJ�ڂ��鋗�����ǂ���
			/// </summary>
			/// <param name="member">�J�ڃ����o�[</param>
			/// <returns>�J�ڂł��鋗���Ȃ�true</returns>
			bool IsBodyBlowRange(const Stator_NormalSlime::TransitionMember& member);

			/// <summary>
			/// �p�����[�^�̐ݒ�
			/// </summary>
			void SettingParametor();

			/// <summary>
			/// �p�j�p�����[�^�ݒ�
			/// </summary>
			void SettingPlowlingParametor();

			/// <summary>
			/// �Ǐ]�p�����[�^�ݒ�
			/// </summary>
			void SettingChaseParametor();

			/// <summary>
			/// �U���p�����[�^�ݒ�
			/// </summary>
			void SettingAttackParametor();

		};

	}
}