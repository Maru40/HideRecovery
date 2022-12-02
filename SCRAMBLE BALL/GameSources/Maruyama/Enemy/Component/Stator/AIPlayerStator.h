
/*!
@file AIPlayerStator.h
@brief AIPlayerStator�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Stator/StatorBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class EyeSearchRange;
	class ObserveIsInEyeTarget;
	class I_TeamMember;
	class TargetManager;

	namespace Enemy {

		class EnemyBase;
		class I_FactionMember;

		namespace Tuple {
			class I_Tupler;
		}

		//--------------------------------------------------------------------------------------
		///	AIPlayerStator�̃X�e�[�g�^�C�v
		//--------------------------------------------------------------------------------------
		enum class AIPlayerStator_StateType {
			None,
			HidePlacePatrol,	//�B���ꏊ�T���B
			Buttle,				//�o�g��
			Goal,				//�S�[����
			Dyning,				//���S��
			Dead,				//���S
		};

		//--------------------------------------------------------------------------------------
		///	AIPlayerStator�̑J�ڏ��������o�[
		//--------------------------------------------------------------------------------------
		struct AIPlayerStator_TransitionMember {
			float hidePatrolEeyRange;	//�B���ꏊ��T���Ă���Ƃ��̎��E�͈�
			float buttleStartEyeRange;	//�o�g���J�n���̎��E�͈�

			AIPlayerStator_TransitionMember();
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
			std::weak_ptr<Tuple::I_Tupler> m_tupler;		//�^�v���X�y�[�X���g����
			std::weak_ptr<TargetManager> m_targetManager;	//�^�[�Q�b�g�Ǘ�
			
		public:
			AIPlayerStator(const std::shared_ptr<GameObject>& objPtr);

			virtual ~AIPlayerStator() = default;

			void OnLateStart() override;

			void CreateNode() override;
			void CreateEdge() override;

			void OnEnable() override;

		private:

			//--------------------------------------------------------------------------------------
			///	�J�ڏ����n
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �o�g���^�[�Q�b�g����
			/// </summary>
			/// <param name="member">�J�ڏ��������o�[</param>
			/// <returns>�o�g���^�[�Q�b�g</returns>
			bool IsFindButtleTarget(const TransitionMember& member);

			/// <summary>
			/// �^�[�Q�b�g�����������Ƃ��̑J��
			/// </summary>
			/// <param name="member">�J�ڏ��������o�[</param>
			/// <returns>�^�[�Q�b�g������������true</returns>
			bool IsLostButtleTarget(const TransitionMember& member);

		};

	}
}