/*!
@file TackleCameraObject.h
@brief TackleCameraObject�w�b�_�@
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�^�b�N���J�����I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class TackleCameraObject : public GameObject
	{
		ex_weak_ptr<GameObject> m_seekTarget = nullptr; //�Ǐ]�^�[�Q�b�g�̃I�u�W�F�N�g

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		/// <param name="seekTarget">�Ǐ]�^�[�Q�b�g�̃I�u�W�F�N�g</param>
		TackleCameraObject(const std::shared_ptr<Stage>& stage, const std::shared_ptr<GameObject>& seekTarget);

		void OnCreate() override;

	private:
		/// <summary>
		/// �Ǐ]�Z�b�e�B���O
		/// </summary>
		/// <param name="target">�Ǐ]����^�[�Q�b�g�̃I�u�W�F�N�g</param>
		void SettingSeek(const std::shared_ptr<GameObject>& target);

		/// <summary>
		/// �����_�Z�b�e�B���O
		/// </summary>
		/// <param name="target">��������^�[�Q�b�g�̃I�u�W�F�N�g</param>
		void SettingLookAt(const std::shared_ptr<GameObject>& target);

	public:

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �Ǐ]�^�[�Q�b�g�̃Z�b�g
		/// </summary>
		/// <param name="target">�Ǐ]����^�[�Q�b�g</param>
		void SetSeekTarget(const std::shared_ptr<GameObject>& target) noexcept;

		/// <summary>
		/// ��������^�[�Q�b�g�̃Z�b�g
		/// </summary>
		/// <param name="target">��������^�[�Q�b�g</param>
		void SetLockAtTarget(const std::shared_ptr<GameObject>& target) noexcept;
	};

}

//endbasecross