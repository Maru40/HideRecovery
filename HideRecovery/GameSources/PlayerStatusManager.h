/*!
@file Pusher.h
@brief Pusherなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	struct DamageData;
	class I_Damaged;

	struct PressedEnergy;
	class I_Pusher;

	class GameTimer;

	//--------------------------------------------------------------------------------------
	/// プレイヤーのステータス
	//--------------------------------------------------------------------------------------
	struct PlayerStatus
	{
		float hp;             // プレイヤーのHP // 100
		float maxHP;          // MaxHP

		float energy;         // タックル用のエネルギー // 100
		float energyRecovery; // エネルギー回復量 // 10/秒
		float energyUsage;    // エネルギー消費量 // 50/秒

		bool isUnrivaled;     //無敵状態かどうか

		/// <summary>
		/// コンストラクタ
		/// </summary>
		PlayerStatus();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="maxHp">最大HP</param>
		/// <param name="energy">エネルギー</param>
		/// <param name="energyRecovery">エネルギー回復量</param>
		/// <param name="energyUsage">エネルギー消費量</param>
		/// <param name="isUnrivaled">無敵状態かどうか</param>
		PlayerStatus(const float& maxHp, const float& energy, const float& energyRecovery, const float& energyUsage, const bool& isUnrivaled);
	};

	//--------------------------------------------------------------------------------------
	/// プレイヤーのステータス管理
	//--------------------------------------------------------------------------------------
	class PlayerStatusManager : public Component, public I_Damaged, public I_Pusher
	{
	public:
		using Status = PlayerStatus;

	protected:
		Status m_status;                    //ステータス
		std::unique_ptr<GameTimer> m_timer; //タイム管理クラス
		float m_damageIntervalTime = 5.0f;  //ダメージを受けた後の無敵時間
		bool m_RecoverEnergy = true;        //Energy自動回復ができるかどうか
		float m_EnergyRecoverRate = 1.0f;   //自動回復のレート

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		PlayerStatusManager(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate() override;

		//--------------------------------------------------------------------------------------
		/// インターフェースの実装
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// ダメージを受ける
		/// </summary>
		/// <param name="data">ダメージデータ</param>
		void Damaged(const DamageData& data) override;

		/// <summary>
		/// エネルギーの消費
		/// </summary>
		/// <param name="energy">消費エネルギー</param>
		void ReduceEnergy(float energy);

		/// <summary>
		/// 回復量減少
		/// </summary>
		/// <param name="rate">現象倍率</param>
		/// <param name="time">時間</param>
		void EnergyRateTime(float rate, float time);

		/// <summary>
		/// エネルギーの回復
		/// </summary>
		void RecoveryEnergy();

		/// <summary>
		/// 潰す処理
		/// </summary>
		/// <param name="energy">潰れるエネルギー</param>
		void Press(const PressedEnergy& energy) override;

	public:
		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// ステータスの設定
		/// </summary>
		/// <param name="status"></param>
		void SetStatus(const Status& status);

		/// <summary>
		/// ステータスの取得
		/// </summary>
		/// <returns>ステータス</returns>
		Status GetStatus() const;

		/// <summary>
		/// エネルギー回復量を設定
		/// </summary>
		/// <param name="rate">エネルギー回復量</param>
		void SetEnergyRecoverRate(float rate) {
			m_EnergyRecoverRate = rate;
		}

		/// <summary>
		/// 死んでいる状態かどうかを返す。
		/// </summary>
		/// <returns>死んでいる状態かどうか</returns>
		bool IsDeath() const;

		/// <summary>
		/// エネルギーが使える状態かどうか
		/// </summary>
		/// <returns>使えるならtrue</returns>
		bool IsUseEnergy() const;
	};

}