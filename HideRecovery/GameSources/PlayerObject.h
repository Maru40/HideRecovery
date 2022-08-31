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
		using DrawComp = PNTBoneModelDraw;

	private:
		std::weak_ptr<GameObject> m_arm;	//アーム

	public:
		PlayerObject(const std::shared_ptr<Stage>& stage);

		virtual void OnCreate() override;

		std::shared_ptr<GameObject> GetArm() const { return m_arm.lock(); }
	};
}