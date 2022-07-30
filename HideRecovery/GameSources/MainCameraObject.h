/*!
@file MainCameraObj.h
@brief MainCameraObj�w�b�_�@
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//�O���錾
	class SeekTPS;

	//--------------------------------------------------------------------------------------
	///	���C���J�����I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class MainCameraObject : public GameObject
	{
		std::shared_ptr<GameObject> m_target;  //�^�[�Q�b�g�̃I�u�W�F�N�g

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		/// <param name="target">�^�[�Q�b�g�̃I�u�W�F�N�g</param>
		MainCameraObject(const std::shared_ptr<Stage>& stage, const std::shared_ptr<GameObject>& target);

		void OnCreate() override;

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �^�[�Q�b�g�̃Z�b�g
		/// </summary>
		/// <param name="target">�^�[�Q�b�g�̃I�u�W�F�N�g</param>
		void SetTarget(const std::shared_ptr<GameObject>& target);
	};

}

//endbasecross