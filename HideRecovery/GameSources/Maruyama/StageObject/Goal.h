/*!
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
	class GameTimer;
	struct CollisionPair;

	//--------------------------------------------------------------------------------------
	/// ゴール管理クラスのパラメータ
	//--------------------------------------------------------------------------------------
	struct Goal_Parametor {
		Team team;				//チームタイプ
		float itemHiderTime;	//アイテム再配置までの時間
		Vec3 timeDrawPosition;	//時間表示場所

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
		Parametor m_param;					//パラメータ

		 //エフェクト表示用クラス
		 //残り時間表示クラス

		std::unique_ptr<GameTimer> m_timer;	//タイム管理クラス

	public:
		Goal(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// 時間更新処理
		/// </summary>
		void TimerUpdate();

		/// <summary>
		/// ゴール成功(ゴールした人の処理)
		/// </summary>
		/// <param name="pair">ゴールをした人</param>
		void SuccessGoal(const CollisionPair& pair);

		/// <summary>
		/// ゴール成功(他のメンバーから通知を受けた時の処理)
		/// </summary>
		/// <param name="team">チームタイプ</param>
		/// <param name="playerNumber">プレイヤーナンバー</param>
		/// <param name="itemId">アイテムID</param>
		/// <param name="hidePosition">隠す場所</param>
		void SuccessGoal(Team team, int playerNumber, int itemId, const Vec3& hidePosition);

		/// <summary>
		/// ポイント加算の通知を行う
		/// </summary>
		void AddPoint(const Team& team);

		/// <summary>
		/// 再配置までのカウントダウン
		/// </summary>
		void StartCountDown(const std::function<void()>& endEvent);

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

		/// <summary>
		/// アイテムを再配置するまでの時間の設定
		/// </summary>
		/// <param name="time">アイテムを再配置するまでの時間</param>
		void SetItemHiderTime(const float time) noexcept { m_param.itemHiderTime = time; }

		/// <summary>
		/// アイテムを再配置するまでの時間の取得
		/// </summary>
		/// <returns>アイテムを再配置するまでの時間</returns>
		float GetItemHiderTime() const noexcept { return m_param.itemHiderTime; }

		/// <summary>
		/// 再配置までの時間を表示する位置
		/// </summary>
		/// <param name="position">再配置までの時間を表示する位置</param>
		void SetTimeDrawPoisition(const Vec3& position) noexcept { m_param.timeDrawPosition = position; }

		/// <summary>
		/// 再配置までの時間を表示する位置の取得
		/// </summary>
		/// <returns>再配置までの時間を表示する位置</returns>
		Vec3 GetTimeDrawPosition() const noexcept { return m_param.timeDrawPosition; }
		
		/// <summary>
		/// オンライン用イベント
		/// </summary>
		/// <param name="playerNumber">プレイヤーナンバー</param>
		/// <param name="eventCode">イベントコード</param>
		/// <param name="bytes">データ</param>
		void OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes) override;

	};

}