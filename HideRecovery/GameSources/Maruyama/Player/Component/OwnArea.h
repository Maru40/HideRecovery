
/*!
@file OwnArea.h
@brief OwnArea�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Watanabe/StageObject/PlayerSpawnPointObject.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class I_TeamMember;

	//--------------------------------------------------------------------------------------
	/// ���w�G���A�̃p�����[�^
	//--------------------------------------------------------------------------------------
	struct OwnArea_Parametor {
		Team team;		//�`�[��
		float radius;	//�G���A�͈͂̔��a

		OwnArea_Parametor(const Team& team, const float radius);
	};

	//--------------------------------------------------------------------------------------
	/// ���w�G���A
	//--------------------------------------------------------------------------------------
	class OwnArea : public Component
	{
	public:
		using Parametor = OwnArea_Parametor;

	private:
		Parametor m_param;	//�p�����[�^

		std::vector<std::weak_ptr<I_TeamMember>> m_members;	//�G���A�ɏ������Ă��郁���o�[�ꗗ

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="parametor">�p�����[�^</param>
		OwnArea(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

		void OnCreate() override;

	private:

	public:
		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �`�[���̐ݒ�
		/// </summary>
		/// <param name="team">�`�[��</param>
		void SetTeam(const Team& team) noexcept { m_param.team = team; }

		/// <summary>
		/// �`�[���̎擾
		/// </summary>
		/// <returns>�`�[��</returns>
		Team GetTeam() const noexcept { return m_param.team; }

		/// <summary>
		/// �G���A���a�̐ݒ�
		/// </summary>
		/// <param name="radius">�G���A���a</param>
		void SetRadius(const float radius) noexcept { m_param.radius = radius; }

		/// <summary>
		/// �G���A���a�̎擾
		/// </summary>
		/// <returns>�G���A���a</returns>
		float GetRadius() const noexcept { return m_param.radius; }

		/// <summary>
		/// �����o�[�̒ǉ�
		/// </summary>
		/// <param name="member">�����o�[</param>
		void AddMember(const std::shared_ptr<I_TeamMember>& member);

		/// <summary>
		/// �����o�[�̍폜
		/// </summary>
		/// <param name="member">�폜���郁���o�[</param>
		bool RemoveMember(const std::shared_ptr<I_TeamMember>& member);

		/// <summary>
		/// ���w�G���A���ɂ��邩�ǂ������f
		/// </summary>
		/// <param name="member">�m�F����肽�������o�[</param>
		/// <returns>���w�G���A���ɂ���Ȃ�true</returns>
		bool IsInArea(const std::shared_ptr<I_TeamMember>& member);

		/// <summary>
		/// ���w�G���A���ɂ��邩�ǂ���
		/// </summary>
		/// <param name="object">�m�F����肽�������o�[</param>
		/// <returns>���w�G���A���ɂ���Ȃ�true</returns>
		bool IsInArea(const std::shared_ptr<GameObject>& member);
	};

}