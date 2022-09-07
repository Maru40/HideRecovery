/*!
@file TactileObject.h
@brief TactileObject
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�G�o�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class TactileObject : public GameObject
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		TactileObject(const std::shared_ptr<Stage>& stage) 
			:GameObject(stage)
		{}

		void OnCreate() override;

	};

}

//endbasecross