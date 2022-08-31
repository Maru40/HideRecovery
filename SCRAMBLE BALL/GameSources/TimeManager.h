/*!
@file TimeManager.h
@brief 時間管理
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//-----------------------------------------------------------------------------------------------
	/// TimeManagerに渡すデータ構造体
	//-----------------------------------------------------------------------------------------------
	struct TimeData
	{
		wstring timerName; //! タイマーの名前
		float time; //! 残り時間
		float endTime; //! 終了時間
		std::function<void()> func; //! 終了時に実行する関数
		bool* flag; //! 終了時に反転するフラグ

		//-----------------------------------------------------------------------------------------------
		/**
		* @brief コンストラクタ
		* @param[in] timerName タイマーを識別するための名前
		* @param[in] endtime タイマー終了までの秒数
		* @param[in] func 終了時に実行する関数
		*/
		TimeData(const wstring& timerName, float endTime, std::function<void()> func)
			: timerName(timerName), time(endTime), endTime(endTime), func(func), flag(NULL)
		{}

		/**
		* @brief コンストラクタ
		* @param[in] timerName タイマーを識別するための名前
		* @param[in] endTime タイマー終了までの秒数
		* @param[in] flag 終了時に反転する変数
		*/
		TimeData(const wstring& timerName, float endTime, bool* flag)
			: timerName(timerName), time(endTime), endTime(endTime), func(), flag(flag)
		{}
		//-----------------------------------------------------------------------------------------------
		/// タイマー終了時の処理
		virtual void EndTimer();
		//-----------------------------------------------------------------------------------------------
	};

	//-----------------------------------------------------------------------------------------------
	/// 時間経過を伴う行動を管理するコンポーネントクラス
	//-----------------------------------------------------------------------------------------------
	class TimeManager : public Component
	{
		std::list<TimeData> m_datas; //! 管理中のタイマー

	public:
		//-----------------------------------------------------------------------------------------------
		/// コンストラクタ
		TimeManager(const std::shared_ptr<GameObject>& ptrObj)
			:Component(ptrObj)
		{}
		//-----------------------------------------------------------------------------------------------
		void OnUpdate() override; //更新
		//-----------------------------------------------------------------------------------------------
		/**
		* @brief タイマーの追加
		* @param[in] data 追加するタイマーのデータ
		* @return 追加に成功したらtrue、失敗したらfalse
		*/
		bool AddTimer(const TimeData& data);

		/**
		* @brief タイマーの再定義
		* @param[in] data 再定義するタイマーのデータ
		* @datail 同じ名前のタイマーのパラメータを書き換える。
		* @datail 同じ名前のタイマーがない場合は新たに追加する。
		*/
		void ReSetTimer(const TimeData& data);

		//-----------------------------------------------------------------------------------------------
		// アクセッサ
		//-----------------------------------------------------------------------------------------------
		/// タイマーの残り時間を得る
		float GetTime(const wstring& str);

		/// 残り時間が全体の何%かを得る
		float GetTimeRate(const wstring& str);
		//-----------------------------------------------------------------------------------------------
	};
}