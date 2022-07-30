/*!
@file TimerDestroy.h
@brief TimerDestroy
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class GameTimer;

	//--------------------------------------------------------------------------------------
	///	一定時間後に削除してくれるコンポーネント
	//--------------------------------------------------------------------------------------
	class TimerDestroy : public Component
	{
		unique_ptr<GameTimer> m_timer; //時間管理クラス

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="time">削除したい時間</param>
		/// <param name="endFunction">終了時に呼び出したいイベント</param>
		TimerDestroy(const std::shared_ptr<GameObject>& objPtr, const float& time, const std::function<void()>& endFunction = nullptr);
		
		void OnUpdate();

	private:

	public:
		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 経過時間 / 設定時間 == 経過時間の割合
		/// </summary>
		/// <returns>経過時間の割合</returns>
		float GetTimeRate() const;

		/// <summary>
		/// 1.0f - ( 経過時間 / 設定時間 )
		/// </summary>
		/// <returns>1.0f - ( 経過時間 / 設定時間 )</returns>
		float GetIntervalTimeRate() const;

	};

}

//endbasecross