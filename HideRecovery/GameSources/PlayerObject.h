#pragma once
/*!
@file PlayerObject.h
@brief PlayerObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class PlayerObject : public GameObject
	{
	public:
		PlayerObject(const std::shared_ptr<Stage>& stage)
			:GameObject(stage)
		{}

		void OnCreate() override {}

	};

}