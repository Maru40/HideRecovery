/*!
@file BoostCenterObject.h
@brief BoostCenterObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �u�[�X�g�̒��S�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class BoostCenterObject : public GameObject
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		BoostCenterObject(const std::shared_ptr<Stage>& stage);

		void OnCreate();
	};

}