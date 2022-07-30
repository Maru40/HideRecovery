
/*!
@file Animator_BigSlime.h
@brief Animator_BigSlime�Ȃ�
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

		namespace Motion {
			struct KnockBack_Parametor;
		}

		namespace BigSlime {

			//--------------------------------------------------------------------------------------
			/// ����X���C���A�j���[�^�[�̃X�e�[�g�^�C�v
			//--------------------------------------------------------------------------------------
			enum class Animator_StateType
			{
				Idle,                //�ҋ@
				IdleWait,            //�U����̍d���Ȃ�
				Walk,                //����
				BodyBlowPreliminary, //�\������
				BodyBlow,            //�{�f�B�[�u���[
				KnockBack,           //�m�b�N�o�b�N
				Pressed,             //�ׂ��
			};

			//--------------------------------------------------------------------------------------
			/// ����X���C���A�j���[�^�[�̑J�ڏ��������o�[
			//--------------------------------------------------------------------------------------
			struct Animator_TransitionMember
			{
				float toWalkSpeed = 0.2f;  //walk�ɑJ�ڂ���X�s�[�h
			};

			//--------------------------------------------------------------------------------------
			/// ����X���C���A�j���[�^�[�u���b�N�{�[�h
			//--------------------------------------------------------------------------------------
			struct Animator_BlackBoard {
				std::shared_ptr<Enemy::Motion::KnockBack_Parametor> knockBackParamPtr; //�m�b�N�o�b�N�p�����[�^
				std::shared_ptr<Enemy::Motion::KnockBack_Parametor> pressedParamPtr;   //�v���X�f�[�^�̃p�����[�^

				Animator_BlackBoard();
			};

			//--------------------------------------------------------------------------------------
			/// ����X���C���A�j���[�^�[
			//--------------------------------------------------------------------------------------
			class Animator : public AnimatorBase<EnemyBase, Animator_StateType, Animator_TransitionMember>, public I_BrackBoard<Animator_BlackBoard>
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

			};

		}
	}
}