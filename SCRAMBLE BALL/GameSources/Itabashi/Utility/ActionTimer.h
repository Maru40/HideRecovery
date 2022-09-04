#pragma once
#include<vector>
#include"DelegateSystem.h"

namespace itbs
{
	namespace Utility
	{
		/// <summary>
		/// ActionTimer登録データ構造体
		/// </summary>
		struct ActionData
		{
			/// <summary>
			/// 実行するイベント
			/// </summary>
			itbs::Utility::Delegate<void()> action;
			/// <summary>
			/// カウントされている時間
			/// </summary>
			float advancedTime;
			/// <summary>
			/// イベントが発行される時間
			/// </summary>
			float invokeTime;
		};

		/// <summary>
		/// イベントのタイマー管理クラス
		/// </summary>
		class ActionTimer
		{
			/// <summary>
			/// イベントデータ配列
			/// </summary>
			std::vector<ActionData> m_actionDatas;

		public:
			/// <summary>
			/// タイマー管理するイベントを登録
			/// </summary>
			/// <param name="actionData">登録するイベントデータ</param>
			void AddAction(const ActionData& actionData);

			/// <summary>
			/// イベントのタイマーを進める
			/// </summary>
			/// <param name="advancedTime">進める時間</param>
			void AdvanceTime(const float advancedTime);
		};
	}
}