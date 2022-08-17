
/*!
@file Respawner.h
@brief Respawner�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class PlayerSpawnPoint;
	class GameTimer;

	//--------------------------------------------------------------------------------------
	/// ���X�|�[����������N���X�̃p�����[�^
	//--------------------------------------------------------------------------------------
	struct Respawner_Parametor {
		float time;	//���X�|�[���ɂ����鎞��

		Respawner_Parametor();
	};

	//--------------------------------------------------------------------------------------
	/// ���X�|�[����������N���X
	//--------------------------------------------------------------------------------------
	class Respawner : public Component
	{
	public:
		using Parametor = Respawner_Parametor;

	private:
		Parametor m_param;

		std::unique_ptr<GameTimer> m_timer;

		std::weak_ptr<PlayerSpawnPoint> m_spawnPoint;

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		Respawner(const std::shared_ptr<GameObject>& objPtr);

		virtual ~Respawner() = default;

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// ���X�|�[��
		/// </summary>
		void Respawn();

	public:
		/// <summary>
		/// ���X�|�[���J�n
		/// </summary>
		void StartRespawn();

		/// <summary>
		/// ���X�|�[���ł��邩�ǂ���
		/// </summary>
		/// <returns>���X�|�[���ł���Ȃ�true</returns>
		bool IsRespawn();

		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �X�|�[���|�C���g�̐ݒ�
		/// </summary>
		/// <param name="spawnPoint">�X�|�[���|�C���g</param>
		void SetSpawnPoint(const std::shared_ptr<PlayerSpawnPoint>& spawnPoint) { m_spawnPoint = spawnPoint; }

		/// <summary>
		/// �X�|�[���|�C���g�̎擾
		/// </summary>
		/// <returns>�X�|�[���|�C���g</returns>
		std::shared_ptr<PlayerSpawnPoint> GetSpawnPoint() const { return m_spawnPoint.lock(); }
	};

}