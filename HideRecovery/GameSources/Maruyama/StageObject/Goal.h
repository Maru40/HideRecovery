/*!
@file Goal.h
@brief Goalクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "../Interface/I_TeamMember.h"
#include "Itabashi/OnlineManager.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class GameTimer;
	class I_Performable;
	struct CollisionPair;
	class Item;

	//--------------------------------------------------------------------------------------
	/// ゴール管理クラスのパラメータ
	//--------------------------------------------------------------------------------------
	struct Goal_Parametor {
		team::TeamType team;					//チームタイプ
		float itemHiderTime;		//アイテム再配置までの時間
		Vec3 timeDrawPosition;		//時間表示場所
		Vec3 dunkPositionOffset;	//ダンクをする位置のオフセット
		Vec3 dunkBallPositionOffset;
		std::vector<Vec3> firePositionOffsets;	//ファイヤーポジションオフセット

		Goal_Parametor(const team::TeamType& team);
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

		std::vector<std::weak_ptr<I_Performable>> m_fireEffets; //エフェクト表示用クラス
		 //残り時間表示クラス

		std::unique_ptr<GameTimer> m_timer;	//タイム管理クラス

		std::weak_ptr<SoundEmitter> m_soundEmitter;

		SoundClip m_goalEffectSoundClip;

	public:
		Goal(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

		void OnLateStart() override;
		void OnUpdate() override;

	private:
		void SettingPerformable();

		/// <summary>
		/// 時間更新処理
		/// </summary>
		void TimerUpdate();

		Vec3 GoalProcess(const std::shared_ptr<GameObject>& other, const std::shared_ptr<Item>& item);

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
		void SuccessGoal(team::TeamType team, int playerNumber, int itemId, const Vec3& hidePosition);

		/// <summary>
		/// ポイント加算の通知を行う
		/// </summary>
		void AddPoint(const team::TeamType& team);

		/// <summary>
		/// 再配置までのカウントダウン
		/// </summary>
		void StartCountDown(const std::function<void()>& endEvent);

		/// <summary>
		/// アニメーション再生
		/// </summary>
		void PlayAnimation(const std::shared_ptr<GameObject>& other);

		/// <summary>
		/// 当たり判定をとるかどうか
		/// </summary>
		/// <param name="pair">当たった相手のデータ</param>
		/// <returns>当たり判定をとるならtrue</returns>
		bool IsCollision(const CollisionPair& pair) const;

	public:
		void OnCollisionEnter(const CollisionPair& pair) override;

		void PlayFireEffects();

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// チームタイプの設定
		/// </summary>
		/// <param name="team">チームタイプ</param>
		void SetTeam(const team::TeamType& team) noexcept { m_param.team = team; }

		/// <summary>
		/// チームタイプの取得
		/// </summary>
		/// <returns>チームタイプ</returns>
		team::TeamType GetTeam() const noexcept { return m_param.team; }

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
		/// ダンク位置を取得できるようにした。
		/// </summary>
		/// <returns>ダンク位置</returns>
		Vec3 GetDunkPosition() const { return transform->GetPosition() + m_param.dunkPositionOffset; }

		/// <summary>
		/// オンライン用イベント
		/// </summary>
		/// <param name="playerNumber">プレイヤーナンバー</param>
		/// <param name="eventCode">イベントコード</param>
		/// <param name="bytes">データ</param>
		void OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes) override;
	};
}