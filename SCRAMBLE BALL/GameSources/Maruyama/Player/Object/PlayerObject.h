﻿/*!
@file PlayerObject.h
@brief PlayerObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"
#include "Watanabe/Shader/BoneModelDraw.h"

namespace basecross {

	class PlayerSpawnPoint;

	//--------------------------------------------------------------------------------------
	/// プレイヤーオブジェクト
	//--------------------------------------------------------------------------------------
	class PlayerObject : public GameObject
	{
	public:
		using DrawComp = BoneModelDraw;

	private:
		std::weak_ptr<GameObject> m_arm;	//アーム

		std::shared_ptr<PlayerSpawnPoint> GetSpawmPoint(int uniqueId) const;

	public:
		PlayerObject(const std::shared_ptr<Stage>& stage);

		virtual void OnCreate() override;

		virtual void OnlineSetting(int gameNumber, int playerNumber);

		std::shared_ptr<GameObject> GetArm() const { return m_arm.lock(); }
	};
}