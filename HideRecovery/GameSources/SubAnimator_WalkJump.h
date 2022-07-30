/*!
@file SubAnimator_WalkJump.h
@brief SubAnimator_WalkJump�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class EnemyBase;

		namespace SubAnimator {

			//--------------------------------------------------------------------------------------
			/// �W�����v�ړ��T�u�A�j���[�^�[�̃X�e�[�g�^�C�v
			//--------------------------------------------------------------------------------------
			enum class WalkJump_StateType
			{
				Idle,        //�ҋ@
				Preliminary, //�\������
				Jump,        //�W�����v
				Wait,        //�d��
			};

			//--------------------------------------------------------------------------------------
			/// �W�����v�ړ��T�u�A�j���[�^�[�̑J�ڏ��������o�[
			//--------------------------------------------------------------------------------------
			struct WalkJump_TransitionMember
			{

			};

			//--------------------------------------------------------------------------------------
			///�W�����v�ړ��T�u�A�j���[�^�[
			//--------------------------------------------------------------------------------------
			class WalkJump : public SubAnimatorBase<EnemyBase, WalkJump_StateType, WalkJump_TransitionMember>
			{
			public:
				using StateType = WalkJump_StateType;
				using TransitionMember = WalkJump_TransitionMember;

			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
				WalkJump(const std::shared_ptr<GameObject>& objPtr);

				void CreateNode() override;
				void CreateEdge() override;

			};

		}
	}
}