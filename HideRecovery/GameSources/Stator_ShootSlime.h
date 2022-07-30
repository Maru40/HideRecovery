
/*!
@file Stator_ShootSlime.h
@brief Stator_ShootSlime�Ȃ�
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
		}

		namespace ShootSlime {

			//--------------------------------------------------------------------------------------
			/// �V���b�g�X���C���̃X�e�[�^�[�̑J�ڏ��������o�[
			//--------------------------------------------------------------------------------------
			struct Stator_ShootSlime_TransitionMember {
				Trigger chaseTrigger;      //�Ǐ]�ɐ؂�ւ��g���K�[
				Trigger knockBackTrigger;  //�m�b�N�o�b�N�g���K�[

				float startChaseRange;     //�Ǐ]���n�߂鋗��
				float maxChaseRange;       //�ǐՂ���ő勗��
				float attackStartRange;    //�U���J�n����

				Stator_ShootSlime_TransitionMember();
				Stator_ShootSlime_TransitionMember(
					const float& startChaseRange,
					const float& maxChaseRange,
					const float& attackStartRange
				);
			};

			//--------------------------------------------------------------------------------------
			/// �V���b�g�X���C���̃X�e�[�^�[�̃u���b�N�{�[�h
			//--------------------------------------------------------------------------------------
			struct Stator_ShootSlime_BlackBoard
			{
				std::shared_ptr<Enemy::StateNode::Slime_Dyning_Parametor> deathParamPtr;  //���S�p�����[�^
			};

			//--------------------------------------------------------------------------------------
			/// �V���b�g�X���C���̃X�e�[�^�[
			//--------------------------------------------------------------------------------------
			class Stator_ShootSlime : public SlimeStatorBase<EnemyBase, Stator_ShootSlime_TransitionMember>, public I_BrackBoard<Stator_ShootSlime_BlackBoard>
			{
			public:
				using TransitionMember = Stator_ShootSlime_TransitionMember;
				using BlackBoard = Stator_ShootSlime_BlackBoard;

				struct Parametor;

			private:
				std::unique_ptr<Parametor> m_paramPtr;  //�p�����[�^

			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
				Stator_ShootSlime(const std::shared_ptr<GameObject>& objPtr);

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
				/// <param name="member">�J�ڏ����p�����o�[</param>
				Stator_ShootSlime(const std::shared_ptr<GameObject>& objPtr, const TransitionMember& member);

			private:
				void CreateNode() override;
				void CreateEdge() override;

				/// <summary>
				/// �p�����[�^�̐ݒ�
				/// </summary>
				void SettingParametor();

				/// <summary>
				/// �p�j�s���p�����[�^�̐ݒ�
				/// </summary>
				void SettingPlowlingParametor();

				/// <summary>
				/// �U���p�����[�^�̐ݒ�
				/// </summary>
				void SettingAttackParametor();

			};
		}
	}
}