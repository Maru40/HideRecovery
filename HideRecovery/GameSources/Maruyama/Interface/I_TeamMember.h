
/*!
@file I_TeamMember.h
@brief I_TeamMember�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Watanabe/StageObject/PlayerSpawnPointObject.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class OwnArea;

	//--------------------------------------------------------------------------------------
	/// �`�[�������o�[�̃C���^�[�t�F�[�X
	//--------------------------------------------------------------------------------------
	class I_TeamMember
	{
		std::weak_ptr<OwnArea> m_ownArea;

	public:
		virtual ~I_TeamMember() = default;

		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

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
		virtual void SetTeam(const Team& team) = 0;

		/// <summary>
		/// �`�[���^�C�v�̎擾
		/// </summary>
		/// <returns>�`�[���^�C�v</returns>
		virtual Team GetTeam() const = 0;
	};

}