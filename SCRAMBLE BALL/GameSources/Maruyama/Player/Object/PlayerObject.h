/*!
@file PlayerObject.h
@brief PlayerObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"
#include "Watanabe/Shader/BoneModelDraw.h"
#include "Watanabe/UI/DirectionWithHasBallUI.h"

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

		std::weak_ptr<DirectionWithHasBallUI> m_directionUI;

		std::shared_ptr<PlayerSpawnPoint> GetSpawmPoint(int uniqueId) const;

	public:
		PlayerObject(const std::shared_ptr<Stage>& stage);

		virtual void OnCreate() override;

		virtual void OnlineSetting(int gameNumber, int playerNumber);

		std::shared_ptr<GameObject> GetArm() const { return m_arm.lock(); }

		// ボールを拾ったときのイベントで使用
		std::shared_ptr<DirectionWithHasBallUI> GetDirectionWithHasBallUI() const { return m_directionUI.lock(); }
	};
}