#pragma once
/*!
@file PlayerObject.h
@brief PlayerObject‚È‚Ç
’S“–FŠÛR—TŠì
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