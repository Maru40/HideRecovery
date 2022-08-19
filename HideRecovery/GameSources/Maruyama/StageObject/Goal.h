﻿/*!
@file Goal.h
@brief Goalクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Watanabe/StageObject/PlayerSpawnPointObject.h"
#include "Itabashi/OnlineManager.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	struct CollisionPair;

	//--------------------------------------------------------------------------------------
	/// ゴール管理クラスのパラメータ
	//--------------------------------------------------------------------------------------
	struct Goal_Parametor {
		Team team;

		Goal_Parametor(const Team& team);
	};

	//--------------------------------------------------------------------------------------
	/// ゴール管理クラス
	//--------------------------------------------------------------------------------------
	class Goal : public Online::OnlineComponent
	{
	public:
		using Parametor = Goal_Parametor;
		static constexpr std::uint8_t EXECUTE_GOAL_EVENT_CODE = 30;

	private:
		Parametor m_param;

	public:
		Goal(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// ゴール成功
		/// </summary>
		/// <param name="pair">ゴールをした人</param>
		void SuccessGoal(const CollisionPair& pair);

		void SuccessGoal(Team team, int playerNumber, int itemId, const Vec3& hidePosition);

		/// <summary>
		/// 当たり判定をとるかどうか
		/// </summary>
		/// <param name="pair">当たった相手のデータ</param>
		/// <returns>当たり判定をとるならtrue</returns>
		bool IsCollision(const CollisionPair& pair) const;

	public:
		void OnCollisionEnter(const CollisionPair& pair) override;

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// チームタイプの設定
		/// </summary>
		/// <param name="team">チームタイプ</param>
		void SetTeam(const Team& team) noexcept { m_param.team = team; }

		/// <summary>
		/// チームタイプの取得
		/// </summary>
		/// <returns>チームタイプ</returns>
		Team GetTeam() const noexcept { return m_param.team; }

		void OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes) override;

	};

}