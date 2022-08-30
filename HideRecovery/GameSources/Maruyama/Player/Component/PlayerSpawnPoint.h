/*!
@file PlayerSpawnPoint.h
@brief PlayerSpawnPoint�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Interface/I_TeamMember.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// �v���C���[���X�|�[�������ʒu�Ǘ��N���X�̃p�����[�^
	//--------------------------------------------------------------------------------------
	struct PlayerSpawnPoint_Parametor {
		team::TeamType team;	// �`�[��
		int id;		// �ŗLID

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="team">�`�[��</param>
		/// <param name="id">�ŗLID</param>
		PlayerSpawnPoint_Parametor(const team::TeamType& team, const int id);
	};

	//--------------------------------------------------------------------------------------
	/// �v���C���[���X�|�[�������ʒu�Ǘ��N���X
	//--------------------------------------------------------------------------------------
	class PlayerSpawnPoint : public Component
	{
	public:
		using Parametor = PlayerSpawnPoint_Parametor;

	private:
		Parametor m_param;	//�p�����[�^

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="parametor">�p�����[�^</param>
		PlayerSpawnPoint(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

	public:
		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �`�[���^�C�v�̐ݒ�
		/// </summary>
		/// <param name="team">�`�[���^�C�v</param>
		void SetTeam(const team::TeamType& team) noexcept { m_param.team = team; }

		/// <summary>
		/// �`�[���^�C�v�̎擾
		/// </summary>
		/// <returns>�`�[���^�C�v</returns>
		team::TeamType GetTeam() const noexcept { return m_param.team; }

		/// <summary>
		/// �ŗLID�̎擾
		/// </summary>
		/// <returns>�ŗLID</returns>
		int GetID() const noexcept { return m_param.id; }

		Vec3 GetWorldPosition() const { return transform->GetWorldPosition(); }

		Quat GetQuaternion()const { return transform->GetQuaternion(); }
	};
}