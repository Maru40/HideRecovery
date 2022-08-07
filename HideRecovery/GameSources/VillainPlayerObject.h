
/*!
@file VillainPlayerObject.h
@brief VillainPlayerObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "PlayerObject.h"

namespace basecross {


	class VillainPlayerObject : public PlayerObject
	{
	public:
		VillainPlayerObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;
	};

}