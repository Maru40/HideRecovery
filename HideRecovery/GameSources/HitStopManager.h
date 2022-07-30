/*!
@file HitStopManager.h
@brief HitStopManager
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "SingletonComponent.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class GameTimer;

	//--------------------------------------------------------------------------------------
	///	ヒットストップ管理
	//--------------------------------------------------------------------------------------
	class HitStopManager : public maru::SingletonComponent<HitStopManager>
	{
		using SingletonComponent = maru::SingletonComponent<HitStopManager>;

	public:
		/// <summary>
		/// ヒットストップで扱うデータ
		/// </summary>
		struct Data 
		{
			float stopTime;     //停止する時間
			float startTime;    //何秒後に開始するか

			Data();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="stopTime">停止する時間</param>
			/// <param name="startTime">何秒後に開始するか</param>
			Data(const float& stopTime, const float& startTime);
		};

	private:
		/// <summary>
		/// パラメータ
		/// </summary>
		struct Parametor
		{
			Data hitStopData;                   //ヒットストップデータ
			float defaultSpeed;                 //通常時のタイマー速度
			std::function<void()> endFunction;  //終了時に呼び出すイベント

			/// <summary>
			/// コンストラクタ
			/// </summary>
			Parametor();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="hitStopData">ヒットストップデータ</param>
			/// <param name="defaultSpeed">通常時のタイマー速度</param>
			/// <param name="endFunction">終了時に呼び出すイベント</param>
			Parametor(const Data& hitStopData, const float& defaultSpeed, const std::function<void()>& endFunction = nullptr);
		};

		Parametor m_param;					//パラメータ

		std::unique_ptr<GameTimer> m_timer;	//タイム管理クラス

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		HitStopManager(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate() override;

	private:
		/// <summary>
		/// ヒットストップ開始
		/// </summary>
		/// <param name="stopTime">ストップさせる時間</param>
		void StartStop(const float& stopTime);

		/// <summary>
		/// カウント終了時に呼び出す処理(ヒットストップ終了)
		/// </summary>
		void EndTimer(); 

	public:
		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// ヒットストップ開始を外部から呼び出す
		/// </summary>
		/// <param name="stopTime">ストップさせる時間</param>
		/// <param name="startTime">ヒットストップを開始する時間</param>
		/// <param name="endFunction">終了時に呼び出すイベント</param>
		void StartTimer(const float& stopTime, const float& startTime = 0.001f, const std::function<void()>& endFunction = nullptr);

		/// <summary>
		/// ヒットストップデータの取得
		/// </summary>
		/// <returns>ヒットストップデータ</returns>
		Data GetStopData() const noexcept;

		/// <summary>
		/// ヒットストップをしているかどうか
		/// </summary>
		/// <returns>ヒットストップ中ならtrue</returns>
		bool IsHitStop() const;
	};

}

//endbasecross