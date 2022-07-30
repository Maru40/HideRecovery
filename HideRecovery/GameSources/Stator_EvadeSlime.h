
/*!
@file Stator_EvadeSlime.h
@brief Stator_EvadeSlime�Ȃ�
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

		namespace EvadeSlime {

			//--------------------------------------------------------------------------------------
			/// ������X���C���X�e�[�^�[�̑J�ڏ��������o�[
			//--------------------------------------------------------------------------------------
			struct Stator_EvadeSlime_TransitionMember {
				Trigger knockBackTrigger;  //�m�b�N�o�b�N�g���K�[

				float startEvadeRange;     //������Ɣ��f���鋗��
				float endEvadeRange;       //�����؂����Ɣ��f���鋗��
				
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				Stator_EvadeSlime_TransitionMember();

				/// <summary>
				/// 
				/// </summary>
				/// <param name="startEvadeRange"></param>
				/// <param name="endEvadeRange"></param>
				Stator_EvadeSlime_TransitionMember(const float& startEvadeRange, const float& endEvadeRange);
			};

			//--------------------------------------------------------------------------------------
			/// ������X���C���X�e�[�^�[�̃u���b�N�{�[�h
			//--------------------------------------------------------------------------------------
			struct Stator_EvadeSlime_BlackBoard
			{
				std::shared_ptr<Enemy::StateNode::Slime_Dyning_Parametor> deathParamPtr;  //���S
			};

			//--------------------------------------------------------------------------------------
			/// ������X���C���̃X�e�[�^�[
			//--------------------------------------------------------------------------------------
			class Stator_EvadeSlime : public SlimeStatorBase<EnemyBase, Stator_EvadeSlime_TransitionMember>, public I_BrackBoard<Stator_EvadeSlime_BlackBoard>
			{
			public:
				using TransitionMember = Stator_EvadeSlime_TransitionMember;

				/// <summary>
				/// �p�����[�^
				/// </summary>
				struct Parametor;

			private:
				std::unique_ptr<Parametor> m_paramPtr = nullptr;  //�p�����[�^

			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
				Stator_EvadeSlime(const std::shared_ptr<GameObject>& objPtr);

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
				/// <param name="member">�J�ڏ��������o�[</param>
				Stator_EvadeSlime(const std::shared_ptr<GameObject>& objPtr, const TransitionMember& member);

			private:
				void CreateNode() override;
				void CreateEdge() override;

				/// <summary>
				/// �p�����[�^�̐ݒ�
				/// </summary>
				void SettingParametor();

				/// <summary>
				/// �p�j�p�����[�^�̐ݒ�
				/// </summary>
				void SettingPlowlingParametor();

			};

		}
	}
}