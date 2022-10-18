
/*!
@file ArrowObject.h
@brief ArrowObject�N���X
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	���I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class ArrowObject : public GameObject
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		ArrowObject(const std::shared_ptr<Stage>& stage)
			:GameObject(stage)
		{}

		void OnCreate() override;

	};

}

//endbasecross