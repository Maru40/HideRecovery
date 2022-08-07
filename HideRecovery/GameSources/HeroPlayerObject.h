
/*!
@file HeroPlayerObject.h
@brief HeroPlayerObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "PlayerObject.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// ヒーロー側のプレイヤーオブジェクト
	//--------------------------------------------------------------------------------------
	class HeroPlayerObject : public PlayerObject
	{
	public:
		HeroPlayerObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;
	};

}