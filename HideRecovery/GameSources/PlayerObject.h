
/*!
@file PlayerObject.h
@brief PlayerObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �v���C���[�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class PlayerObject : public GameObject
	{
	public:
		PlayerObject(const std::shared_ptr<Stage>& stage);

		virtual void OnCreate() override;

	};

}