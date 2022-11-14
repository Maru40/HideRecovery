/*!
@file I_TeamMember.h
@brief I_TeamMember�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Utility/ReactiveBool.h"

#include "Maruyama/Utility/Utility.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class OwnArea;

	namespace team {
		/// <summary>
		/// �`�[���^�C�v
		/// </summary>
		enum class TeamType {
			Blue,
			Red,
			Neutral
		};

		// �`�[�������o�[���iRed��Blue�̍��v�j
		constexpr UINT TEAM_MEMBER_COUNT = 6;

		// �e�`�[���̐F�i�����܂��ɂȂ邱�Ƃ�����̂�basecross::�Ŗ�������j
		static const Col4 REDTEAM_COLOR = basecross::maru::Utility::ConvertColorZeroToOne(Col4(243.0f, 17.0f, 67.0f, 255.0f));
		static const Col4 BLUETEAM_COLOR = basecross::maru::Utility::ConvertColorZeroToOne(Col4(0.0f, 150.0f, 226.0f, 255.0f));

		/// <summary>
		/// �`�[���̐F���擾
		/// </summary>
		/// <param name="teamType">�`�[���^�C�v</param>
		/// <returns>�`�[���̐F</returns>
		Col4 GetTeamColor(TeamType teamType);

		/// <summary>
		/// Enum�𕶎���ɕϊ�
		/// </summary>
		/// <param name="teamType">�`�[���^�C�v</param>
		/// <returns>�`�[���^�C�v�̕�����</returns>
		wstring GetTeamTypeString(TeamType teamType);
	}

	//--------------------------------------------------------------------------------------
	/// �`�[�������o�[�̃C���^�[�t�F�[�X
	//--------------------------------------------------------------------------------------
	class I_TeamMember
	{
		maru::ReactiveBool m_isInArea = true;	//���w�ɂ��邩�ǂ���

		std::weak_ptr<OwnArea> m_ownArea;

	public:
		virtual ~I_TeamMember() = default;

		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// ���w�G���A�ɂ��邩�ǂ����̐ݒ�
		/// </summary>
		/// <param name="isInArea">���w�G���A�����ǂ���</param>
		void SetIsInArea(const bool isInArea) noexcept { m_isInArea = isInArea; }

		/// <summary>
		/// ���w�G���A�ɂ��邩�ǂ���
		/// </summary>
		/// <returns>���w�G���A�ɂ���Ȃ�true</returns>
		bool IsInArea() const noexcept { return m_isInArea.GetValue(); }

		void AddReactiveIsInAreaEvent(const bool b, const std::function<void()>& func) {
			m_isInArea.AddFunction(b, func);
		}

		/// <summary>
		/// ���w�G���A�̐ݒ�
		/// </summary>
		/// <param name="area">���w�G���A</param>
		void SetOwnArea(const std::shared_ptr<OwnArea>& area);

		/// <summary>
		/// ���w�G���A�̎擾
		/// </summary>
		/// <returns>���w�G���A</returns>
		std::shared_ptr<OwnArea> GetOwnArea() const;

		/// <summary>
		/// �Q�[���I�u�W�F�N�g���擾
		/// </summary>
		/// <returns>�Q�[���I�u�W�F�N�g</returns>
		virtual std::shared_ptr<GameObject> GetOwnerObject() const = 0;

		/// <summary>
		/// �`�[���^�C�v�̐ݒ�
		/// </summary>
		/// <param name="team">�`�[���^�C�v</param>
		virtual void SetTeam(const team::TeamType& team) = 0;

		/// <summary>
		/// �`�[���^�C�v�̎擾
		/// </summary>
		/// <returns>�`�[���^�C�v</returns>
		virtual team::TeamType GetTeam() const = 0;
	};
}