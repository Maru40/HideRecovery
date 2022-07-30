
/*!
@file Animator_EvadeSlime.h
@brief Animator_EvadeSlime�Ȃ�
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

		namespace EvadeSlime {

			//--------------------------------------------------------------------------------------
			/// ������X���C���̃A�j���[�^�[�X�e�[�g�^�C�v
			//--------------------------------------------------------------------------------------
			enum class Animator_StateType
			{
				Idle,      //�ҋ@
				IdleWait,  //�U����̍d���Ȃ�
				KnockBack, //�m�b�N�o�b�N
				Pressed,   //�ׂ����
				Walk,      //���s
			};

			//--------------------------------------------------------------------------------------
			/// �A�j���[�^�[�̑J�ڏ��������o�[
			//--------------------------------------------------------------------------------------
			struct Animator_TransitionMember
			{
				float toWalkSpeed = 0.2f;  //walk�ɑJ�ڂ���X�s�[�h
			};

			//--------------------------------------------------------------------------------------
			/// �A�j���[�^�[�̃u���b�N�{�[�h
			//--------------------------------------------------------------------------------------
			struct Animator_BlackBoard {
				std::shared_ptr<Enemy::Motion::KnockBack_Parametor> knockBackParamPtr; //�m�b�N�o�b�N�p�����[�^
				std::shared_ptr<Enemy::Motion::KnockBack_Parametor> pressedParamPtr;   //�ׂ���鎞�̃p�����[�^

				Animator_BlackBoard();
			};

			//--------------------------------------------------------------------------------------
			/// ������X���C���̃A�j���[�^
			//--------------------------------------------------------------------------------------
			class Animator : public AnimatorBase<EnemyBase, Animator_StateType, Animator_TransitionMember>
			{
			public:
				using StateType = Animator_StateType;
				using TransitionMember = Animator_TransitionMember;
				using BlackBoard = Animator_BlackBoard;

			private:
				BlackBoard m_blackBoard;   //�u���b�N�{�[�h

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
				void SetBlackBoard(const BlackBoard& blackBoard);

				/// <summary>
				/// �u���b�N�{�[�h�̎擾
				/// </summary>
				/// <returns>�u���b�N�{�[�h</returns>
				BlackBoard GetBlackBoard() const;

				/// <summary>
				/// �u���b�N�{�[�h�̎Q�Ƃ��擾
				/// </summary>
				/// <returns>�u���b�N�{�[�h�̎Q��</returns>
				BlackBoard& GetRefBlackBoard();

			};

		}
	}
}