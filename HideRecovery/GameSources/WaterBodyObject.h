/*!
@file WaterBodyObject.h
@brief WaterBodyObject
�S���F�ێR �T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �����W�F�b�g�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class WaterBodyObject : public GameObject
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		WaterBodyObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;
	};

}