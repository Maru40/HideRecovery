
/*!
@file VillainPlayerObject.h
@brief VillainPlayerObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "PlayerObject.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �G���̃v���C���[�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class VillainPlayerObject : public PlayerObject
	{
	public:
		VillainPlayerObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;
	};

}