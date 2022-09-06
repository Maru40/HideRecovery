/*!
@file RayMarchingFilterObject.h
@brief RayMarchingFilterObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// ���C�}�[�`���O�t�B���^�[�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class RayMarchingFilterObject : public GameObject
	{
		ex_weak_ptr<Camera> m_target;  //�^�[�Q�b�g

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X�̏�������X�e�[�W</param>
		RayMarchingFilterObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// �f�o�b�O�h���[
		/// </summary>
		void DebugDraw();

		/// <summary>
		/// �Ǐ]�ݒ�
		/// </summary>
		void SettingSeek();
	};


}

//endbaescross