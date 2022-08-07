
/*!
@file PlayerObject.h
@brief PlayerObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// プレイヤーオブジェクト
	//--------------------------------------------------------------------------------------
	class PlayerObject : public GameObject
	{
	public:
		PlayerObject(const std::shared_ptr<Stage>& stage);

		virtual void OnCreate() override;

	};

}