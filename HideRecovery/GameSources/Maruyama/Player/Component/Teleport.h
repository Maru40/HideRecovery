
/*!
@file Teleport.h
@brief Teleport�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �e���|�[�g�@�\
	//--------------------------------------------------------------------------------------

	struct Teleport_Parametor 
	{
		Vec3 position;	//�e���|�[�g����ʒu
	};

	//--------------------------------------------------------------------------------------
	/// �e���|�[�g�@�\
	//--------------------------------------------------------------------------------------
	class Teleport : public Component
	{
	public:
		using Parametor = Teleport_Parametor;

	private:
		Parametor m_param;

	public:
		Teleport(const std::shared_ptr<GameObject>& objPtr);

		void OnLateStart() override;
		void OnUpdate() override;

	private:


	public:
		/// <summary>
		/// �e���|�[�g�J�n
		/// </summary>
		void StartTeleport();

		/// <summary>
		/// �}�b�v���J���B
		/// </summary>
		void OpenMap();

		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �e���|�[�g�ʒu�̐ݒ�
		/// </summary>
		/// <param name="position">�e���|�[�g�ʒu�̐ݒ�</param>
		void SetTeleportPosition(const Vec3& position) noexcept { m_param.position = position; }

		/// <summary>
		/// �e���|�[�g�ʒu�̎擾
		/// </summary>
		/// <returns>�e���|�[�g�ʒu</returns>
		Vec3 GetTeleportPosition() const noexcept { return m_param.position; }
	};

}