
/*!
@file VillainPlayerObject.h
@brief VillainPlayerObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "PlayerObject.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 敵側のプレイヤーオブジェクト
	//--------------------------------------------------------------------------------------
	class VillainPlayerObject : public PlayerObject
	{
	public:
		VillainPlayerObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;
	};

}