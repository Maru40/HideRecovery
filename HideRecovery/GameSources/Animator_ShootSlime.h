
/*!
@file Animator_ShootSlime.h
@brief Animator_ShootSlime�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Trigger.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class EnemyBase;

		namespace Motion {
			struct KnockBack_Parametor;
		}

		namespace ShootSlime {

			//--------------------------------------------------------------------------------------
			/// �V���b�g�X���C���̃A�j���[�^�[�^�C�v
			//--------------------------------------------------------------------------------------
			enum class Animator_StateType
			{
				Idle,      //�ҋ@���
				IdleWait,  //�U����̍d���Ȃ�
				Walk,      //���s���
				KnockBack, //�m�b�N�o�b�N���
				Pressed,   //�ׂ����
			};

			//--------------------------------------------------------------------------------------
			/// �V���b�g�X���C���̃A�j���[�^�[�J�ڏ��������o�[
			//--------------------------------------------------------------------------------------
			struct Animator_TransitionMember
			{
				float toWalkSpeed = 0.2f;  //walk�ɑJ�ڂ���X�s�[�h
			};

			//--------------------------------------------------------------------------------------
			/// �V���b�g�X���C���̃A�j���[�^�[�̃u���b�N�{�[�h
			//--------------------------------------------------------------------------------------
			struct Animator_BlackBoard {
				std::shared_ptr<Enemy::Motion::KnockBack_Parametor> knockBackParamPtr;  //�m�b�N�o�b�N�p�����[�^
				std::shared_ptr<Enemy::Motion::KnockBack_Parametor> pressedParamPtr;    //�ׂ��p�����[�^

				Animator_BlackBoard();
			};

			//--------------------------------------------------------------------------------------
			/// �V���b�g�X���C���̃A�j���[�^�[
			//--------------------------------------------------------------------------------------
			class Animator : public AnimatorBase<EnemyBase, Animator_StateType, Animator_TransitionMember>
			{
			public:
				using StateType = Animator_StateType;
				using TransitionMember = Animator_TransitionMember;
				using BlackBoard = Animator_BlackBoard;

			private:
				BlackBoard m_blackBoard; //�u���b�N�{�[�h

			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
				Animator(const std::shared_ptr<GameObject>& objPtr);

			private:
				void CreateNode() override;
				void CreateEdge() override;

			public:
				//--------------------------------------------------------------------------------------
				/// �A�N�Z�b�T
				//--------------------------------------------------------------------------------------

				/// <summary>
				/// �u���b�N�{�[�h�̐ݒ�
				/// </summary>
				/// <param name="blackBoard">�u���b�N�{�[�h</param>
				void SetBlackBoard(const BlackBoard& blackBoard) noexcept;

				/// <summary>
				/// �u���b�N�{�[�h�̎擾
				/// </summary>
				/// <returns>�u���b�N�{�[�h</returns>
				BlackBoard GetBlackBoard() const noexcept;

				/// <summary>
				/// �u���b�N�{�[�h�̎Q�Ƃ̎擾
				/// </summary>
				/// <returns>�u���b�N�{�[�h�̎Q��</returns>
				BlackBoard& GetRefBlackBoard();

			};

		}
	}
}