/*!
@file SlowTimerManager.h
@brief SlowTimerManagerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "SingletonComponent.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	struct PressData;

	//--------------------------------------------------------------------------------------
	/// スロータイマーパラメータ
	//--------------------------------------------------------------------------------------
	struct SlowTimeParametor
	{
		float time;      //スローにする時間
		float timeSpeed; //スロースピード

		SlowTimeParametor();
		SlowTimeParametor(const float& time, const float& timeSpeed);
	};

	//--------------------------------------------------------------------------------------
	/// タイムカウントのスピードタイプ
	//--------------------------------------------------------------------------------------
	enum class SlowTimerSpeedType {
		Normal, //通常カウント
		Lerp,   //調整中のため使用禁止
	};

	//--------------------------------------------------------------------------------------
	/// スロータイマー管理
	//--------------------------------------------------------------------------------------
	class SlowTimerManager : public maru::SingletonComponent<SlowTimerManager>
	{
	public:
		/// <summary>
		/// パラメータ
		/// </summary>
		struct Parametor
		{
			using SpeedType = SlowTimerSpeedType;

			float defaultTimeSpeed = 1.0f;  //ゲームの通常再生速度
			SlowTimeParametor timeParam;    //スロータイムパラメータ
			SpeedType speedType;            //再生スピードタイプ

			Parametor();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="defaultTimeSpeed">通常再生速度</param>
			/// <param name="timeParam">スロータイムパラメータ</param>
			Parametor(const float& defaultTimeSpeed, const SlowTimeParametor& timeParam);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="defaultTimeSpeed">通常再生速度</param>
			/// <param name="timeParam">スロータイムパラメータ</param>
			/// <param name="speedType">再生スピードタイプ</param>
			Parametor(const float& defaultTimeSpeed, const SlowTimeParametor& timeParam, const SpeedType& speedType);
		};

	private:
		Parametor m_param;                  //パラメータ
		std::unique_ptr<GameTimer> m_timer; //タイム管理クラス

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		SlowTimerManager(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate() override;
		void OnDestroy() override;

	private:
		/// <summary>
		/// 再生速度を返す
		/// </summary>
		/// <returns>再生速度</returns>
		float CalculateUpdateTimeSpeed();

		/// <summary>
		/// 通常の再生速度を返す
		/// </summary>
		/// <returns>通常の再生速度</returns>
		float CalculateNormalTimerSpeed();

		/// <summary>
		/// 補完された再生速度を返す
		/// </summary>
		/// <returns>補完された再生速度</returns>
		float CalculateLerpTimerSpeed();

		/// <summary>
		/// タイムアップ時に呼び出す処理
		/// </summary>
		void EndTime();
		
	public:
		/// <summary>
		/// スロータイムスタート
		/// </summary>
		/// <param name="time">スローにする時間</param>
		/// <param name="slowTimeSpeed">スロー中のスピード</param>
		void StartTimer(const float& time, const float& slowTimeSpeed = 0.3f);

		/// <summary>
		/// 強制ストップ
		/// </summary>
		void ForceStop();

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// Slow状態かどうかを返す。
		/// </summary>
		/// <returns>Slow状態ならtrue</returns>
		bool IsSlow() const;

		/// <summary>
		/// スロータイムパラメータの取得
		/// </summary>
		/// <returns>スロータイムパラメータ</returns>
		SlowTimeParametor GetSlowTimeParametor() const;

		/// <summary>
		/// 通常再生速度の取得
		/// </summary>
		/// <returns>通常再生速度</returns>
		float GetDefaultTimeSpeed() const noexcept {
			return m_param.defaultTimeSpeed;
		}
	};

}