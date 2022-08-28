
/*!
@file Teleport.h
@brief Teleport�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class FieldMap;
	class GameTimer;
	class VirtualCamera;

	//--------------------------------------------------------------------------------------
	/// �e���|�[�g�@�\�̃p�����[�^
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
		Parametor m_param;						//�p�����[�^

		std::weak_ptr<FieldMap> m_fieldMap;		//�}�b�v
		std::unique_ptr<GameTimer> m_timer;		//�^�C�}�[

		std::weak_ptr<VirtualCamera> m_camera;	//�J����

	public:
		Teleport(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnLateStart() override;
		void OnUpdate() override;

	private:
		void SettingFieldMap();
		void SettingAnimationEvent();

	public:
		/// <summary>
		/// �e���|�[�g�J�n
		/// </summary>
		void StartTeleport();

		/// <summary>
		/// �}�b�v���J���B
		/// </summary>
		void OpenMap();

		/// <summary>
		/// �}�b�v�����
		/// </summary>
		void CloseMap();

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

		/// <summary>
		/// �t�B�[���h�}�b�v�̎擾
		/// </summary>
		/// <returns>�t�B�[���h�}�b�v</returns>
		std::shared_ptr<FieldMap> GetFieldMap() const;

		/// <summary>
		/// �e���|�[�g�ł��邩�ǂ���
		/// </summary>
		/// <returns>�e���|�[�g�ł���Ȃ�true</returns>
		bool IsTeleport() const;


		std::shared_ptr<VirtualCamera> GetTeleportCamera() const;
	};

}