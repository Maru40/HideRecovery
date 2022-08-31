/*!
@file Task_Wait.h
@brief Task_Waitなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class GameTimer;

	namespace maru {
		enum class DeltaType;
	}

	namespace Task {

		//--------------------------------------------------------------------------------------
		/// 待機タスクのパラメータ
		//--------------------------------------------------------------------------------------
		struct Wait_Parametor
		{
			float time;                   //待機する時間
			float timeUpdateSpeed;        //時間計測速度
			maru::DeltaType deltaType;    //時間計測タイプ
			std::function<void()> start;  //開始時に呼び出すイベント
			std::function<bool()> update; //更新時に呼び出すイベント
			std::function<void()> exit;   //終了時に呼び出すイベント

			Wait_Parametor();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="time">待機時間</param>
			Wait_Parametor(const float& time);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="time">待機時間</param>
			/// <param name="timeUpdateSpeed">時間計測速度</param>
			/// <param name="deltaType">時間計測タイプ</param>
			Wait_Parametor(const float& time, const float& timeUpdateSpeed, const maru::DeltaType& deltaType);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="time">待機時間</param>
			/// <param name="start">開始時イベント</param>
			/// <param name="update">更新時イベント</param>
			/// <param name="exit">終了時イベント</param>
			Wait_Parametor(const float& time,
				const std::function<void()>& start,
				const std::function<bool()>& update,
				const std::function<void()>& exit
			);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="time">待機時間</param>
			/// <param name="timeUpdateSpeed">時間計測速度</param>
			/// <param name="deltaType">時間計測タイプ</param>
			/// <param name="start">開始時イベント</param>
			/// <param name="update">更新時イベント</param>
			/// <param name="exit">終了時イベント</param>
			Wait_Parametor(const float& time,
				const float& timeUpdateSpeed, 
				const maru::DeltaType& deltaType,
				const std::function<void()>& start,
				const std::function<bool()>& update,
				const std::function<void()>& exit
			);
		};

		//--------------------------------------------------------------------------------------
		/// 待機タスク
		//--------------------------------------------------------------------------------------
		class Wait : public I_TaskNode
		{
		public:
			using Parametor = Wait_Parametor;

		private:
			std::shared_ptr<Parametor> m_paramPtr;  //パラメータ

			std::unique_ptr<GameTimer> m_timer;     //タイマー管理クラス

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="time">待機時間</param>
			Wait(const float& time);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="parametor">パラメータ</param>
			Wait(const Parametor& parametor);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="paramPtr">パラメータのポインタ</param>
			Wait(const std::shared_ptr<Parametor>& paramPtr);

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		};

	}
	
}