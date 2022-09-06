
/*!
@file Utility_EnemyStator.h
@brief Utility_EnemyStator�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	namespace maru {
		template<class node_type, class EnumType, class TransitionStructMember>
		class AnimationMainStateMachine;
	}

	class VelocityManager;

	namespace Enemy {

		namespace UtilityStator {

			//--------------------------------------------------------------------------------------
			/// ��r�^�C�v
			//--------------------------------------------------------------------------------------
			enum class ComparisonType
			{
				Less,     //����
				OrLess,   //�ȉ�
				Greater,  //�傫��
				OrGreater //�ȏ�
			};

			//--------------------------------------------------------------------------------------
			/// �J�ڔ��f�N���X
			//--------------------------------------------------------------------------------------
			class Transition
			{
			public:
				/// <summary>
				/// �v���C���[�����E�ɓ��������̑J�ڔ���
				/// </summary>
				/// <param name="selfObject">�������g�̃I�u�W�F�N�g</param>
				/// <param name="eyeRange">���E�͈�</param>
				/// <returns>�v���C���[�����E�ɓ�������true</returns>
				static bool InEyeRangePlayer(const std::shared_ptr<GameObject>& selfObject, const float& eyeRange);

				/// <summary>
				/// �^�[�Q�b�g�����E�ɓ��������̑J�ڔ���
				/// </summary>
				/// <param name="selfObject">�������g�̃I�u�W�F�N�g</param>
				/// <param name="eyeRange">���E�͈�</param>
				/// <returns>�^�[�Q�b�g�����E�ɓ���������true</returns>
				static bool InEyeRangeTarget(const std::shared_ptr<GameObject>& selfObject, const float& eyeRange);

				/// <summary>
				/// �^�[�Q�b�g�����X�g�����J�ڔ���
				/// </summary>
				/// <param name="selfObject">�������g�̃I�u�W�F�N�g</param>
				/// <param name="maxChaseRange">�Ǐ]�ő勗��</param>
				/// <returns>�Ǐ]�ő勗���O�Ȃ�true</returns>
				static bool LostTarget(const std::shared_ptr<GameObject>& selfObject, const float& maxChaseRange);

				/// <summary>
				/// ���x���r���đJ�ڂ𔻒f
				/// </summary>
				/// <param name="selfObject">�������g�̃I�u�W�F�N�g</param>
				/// <param name="transitionVelocity">�J�ڂ��鑬�x</param>
				/// <param name="type">��r�^�C�v</param>
				/// <returns>��r�^�C�v�ɍ��킹�ď����ɍ��v������true</returns>
				static bool ComparisonVelocity(const std::shared_ptr<GameObject>& selfObject,
					const float& transitionVelocity,
					const ComparisonType& type);

				/// <summary>
				/// �������g���I��������(EnemyMainStateMachine�ł͂܂���Ή�)
				/// </summary>
				/// <param name="stateMachine">�X�e�[�g�}�V��</param>
				/// <param name="state">�I���𔻒f�������X�e�[�g</param>
				/// <returns>�X�e�[�g���I�����Ă�����true</returns>
				template<class node_type, class StateEnum, class TransitionMember>
				static bool IsSelfSetateEnd(
					const std::unique_ptr<maru::AnimationMainStateMachine<node_type, StateEnum, TransitionMember>>& stateMachine,
					const StateEnum& state)
				{
					//�X�e�[�g���������g
					if (stateMachine->IsCurrentState(state)) {
						return stateMachine->GetNowNode()->GetAnimationClip()->IsEnd();  //�X�e�[�g���I��������B
					}

					return false; 
				}

			};
		}
	}
}