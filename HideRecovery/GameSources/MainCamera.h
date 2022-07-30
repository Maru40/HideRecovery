/*!
@file MainCamera.h
@brief ���C���J�����w�b�_�@
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	���C���J����
	//--------------------------------------------------------------------------------------
	class MainCamera : public Camera
	{
		std::weak_ptr<GameObject> m_target; //�^�[�Q�b�g�̃Q�[���I�u�W�F�N�g

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="target">�^�[�Q�b�g�̃Q�[���I�u�W�F�N�g</param>
		MainCamera(const std::weak_ptr<GameObject>& target);

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~MainCamera() {}

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �^�[�Q�b�g�̃Z�b�g
		/// </summary>
		/// <param name="target">�^�[�Q�b�g�\�̃Q�[���I�u�W�F�N�g</param>
		void SetTarget(const std::shared_ptr<GameObject>& target);

	};

}