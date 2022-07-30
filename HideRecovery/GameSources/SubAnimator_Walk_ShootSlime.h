
/*!
@file SubAnimator_Walk_ShootSlime.h
@brief SubAnimator_Walk_ShootSlime�Ȃ�
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

		namespace ShootSlime {

			namespace SubAnimator {

				//--------------------------------------------------------------------------------------
				/// �ړ��T�u�A�j���[�^�\�̃X�e�[�g�^�C�v
				//--------------------------------------------------------------------------------------
				enum class Walk_StateType
				{
					Idle,        //�ҋ@
					Preliminary, //�\������
					Jump,        //�W�����v
					Smooth,      //�L�т�
					Wait,        //�d��
				};

				//--------------------------------------------------------------------------------------
				/// �ړ��T�u�A�j���[�^�[�̑J�ڏ��������o�[
				//--------------------------------------------------------------------------------------
				struct Walk_TransitionMember
				{

				};

				//--------------------------------------------------------------------------------------
				/// �ړ��T�u�A�j���[�^�\
				//--------------------------------------------------------------------------------------
				class Walk : public SubAnimatorBase<EnemyBase, Walk_StateType, Walk_TransitionMember>
				{
				public:
					using StateType = Walk_StateType;
					using TransitionMember = Walk_TransitionMember;

				public:
					/// <summary>
					/// �R���X�g���N�^
					/// </summary>
					/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
					Walk(const std::shared_ptr<GameObject>& objPtr);

					void CreateNode() override;
					void CreateEdge() override;

				};

			}

		}
	}
}