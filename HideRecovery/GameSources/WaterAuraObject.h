/*!
@file WaterAuraObject.h
@brief WaterAuraObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "BulletBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �����W�F�b�g�̃I�[���I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class WaterAuraObject : public GameObject
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		WaterAuraObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;
	};

}