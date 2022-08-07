
/*!
@file HeroPlayerObject.h
@brief HeroPlayerObject‚È‚Ç
’S“–FŠÛR—TŠì
*/

#pragma once
#include "stdafx.h"

#include "PlayerObject.h"

namespace basecross {

	class HeroPlayerObject : public PlayerObject
	{
	public:
		HeroPlayerObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;
	};

}