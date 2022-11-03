
/*!
@file AIPlayerStator.h
@brief AIPlayerStator�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "StatorBase.h"
#include "Maruyama/Enemy/StateMachine/EnemyMainStateMachine.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class EyeSearchRange;
	class ObserveIsInEyeTarget;
	class I_TeamMember;

	namespace Enemy {

		class EnemyBase;
		class I_FactionMember;

		//--------------------------------------------------------------------------------------
		///	AIPlayerStator�̃X�e�[�g�^�C�v
		//--------------------------------------------------------------------------------------
		enum class AIPlayerStator_StateType {
			None,
			HidePlacePatrol,	//�B���ꏊ�T���B
			Buttle,				//�o�g��
		};

		//--------------------------------------------------------------------------------------
		///	AIPlayerStator�̑J�ڏ��������o�[
		//--------------------------------------------------------------------------------------
		struct AIPlayerStator_TransitionMember {
			float hidePatrolEeyRange = 5.0f;	//�B���ꏊ��T���Ă���Ƃ��̎��E�͈�

			AIPlayerStator_TransitionMember() = default;
		};

		//--------------------------------------------------------------------------------------
		///	AIPlayerStator
		//--------------------------------------------------------------------------------------
		class AIPlayerStator : public StatorBase<EnemyBase, AIPlayerStator_StateType, AIPlayerStator_TransitionMember>
		{
		public:
			using StateType = AIPlayerStator_StateType;
			using TransitionMember = AIPlayerStator_TransitionMember;

		private:
			std::weak_ptr<EyeSearchRange> m_eye;			//���E�Ǘ�
			std::weak_ptr<I_TeamMember> m_teamMember;		//�`�[�������o�[�C���^�[�t�F�[�X
			std::weak_ptr<I_FactionMember> m_factionMember;	//�t�@�N�V���������o�[�C���^�[�t�F�[�X

			std::unique_ptr<ObserveIsInEyeTarget> m_observeButtleTarget;	//�o�g���p�ɊĎ��������Ώ�

		public:
			AIPlayerStator(const std::shared_ptr<GameObject>& objPtr);

			virtual ~AIPlayerStator() = default;

			void OnLateStart() override;

			void CreateNode() override;
			void CreateEdge() override;

		private:

			/// <summary>
			/// �o�g���ΏۂƂ��ĊĎ�����^�[�Q�b�g���Z�b�e�B���O
			/// </summary>
			void SettingObserveButtleTargets();

			//--------------------------------------------------------------------------------------
			///	�J�ڏ����n
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �o�g���^�[�Q�b�g����
			/// </summary>
			/// <param name="member">�J�ڏ��������o�[</param>
			/// <returns>�o�g���^�[�Q�b�g</returns>
			bool IsFindButtleTarget(const TransitionMember& member);

		};

	}
}