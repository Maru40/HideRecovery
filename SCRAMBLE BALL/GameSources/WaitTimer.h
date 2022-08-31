
/*!
@file WaitTimer.h
@brief WaitTimerクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	using namespace maru;

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	namespace maru {
		enum class DeltaType;
	}

	//--------------------------------------------------------------------------------------
	///	待機時間パラメータ
	//--------------------------------------------------------------------------------------
	struct WaitTimerParametor
	{
		float time = 1.0f;							//待つ時間
		float timeElapsed = 0;						//時間経過
		float countSpeed = 1.0f;					//更新時間速度
		DeltaType deltaType = DeltaType(0);			//計測時間タイプ
		std::function<void()> endAction = nullptr;  //終了時イベント
		bool isEnd = false;							//終了しているかどうか

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="time">待つ時間</param>
		/// <param name="countSpeed">更新時間速度</param>
		/// <param name="deltaType">計測時間タイプ</param>
		/// <param name="endAction">終了時イベント</param>
		WaitTimerParametor(const float& time, const float& countSpeed,
			const DeltaType& deltaType = DeltaType(0),
			const std::function<void()>& endAction = nullptr
		) :
			time(time), deltaType(deltaType), countSpeed(countSpeed), endAction(endAction)
		{}

		/// <summary>
		/// タイム終了時にする処理
		/// </summary>
		/// <param name="isEndAction">終了時イベントを呼び出すかどうか</param>
		void EndTimer(const bool isEndAction = true) {
			timeElapsed = time;
			isEnd = true;

			if (isEndAction) {
				if (endAction) {
					endAction();
				}
			}

			endAction = nullptr;
		}
	};

	//--------------------------------------------------------------------------------------
	///	待機時間コンポーネント
	//--------------------------------------------------------------------------------------
	class WaitTimer : public Component
	{
		std::map<type_index, std::shared_ptr<WaitTimerParametor>> m_params;	//パラメータ配列

		/// <summary>
		/// 時間更新
		/// </summary>
		void TimerUpdate();

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		WaitTimer(const std::shared_ptr<GameObject>& objPtr)
			:Component(objPtr)
		{}

		void OnUpdate() override;

		/// <summary>
		/// 強制的に待機状態を解除する時に使用する関数
		/// </summary>
		/// <param name="tPtr">待機処理を行いたいclassのポインタ</param>
		/// <param name="time">待機する時間</param>
		/// <param name="endAction">終了時に呼び出す関数</param>
		/// <param name="countSpeed">時間をカウントするスピード</param>
		template<class T>
		void AddTimer(const std::shared_ptr<T> tPtr,
			const float& time, 
			const std::function<void()>& endAction = nullptr, 
			const float& countSpeed = 1.0f,
			const maru::DeltaType & deltaType = DeltaType(0))
		{
			auto tIndex = type_index(typeid(tPtr));

			auto newData = make_shared<WaitTimerParametor>(time, countSpeed, deltaType, endAction);
			m_params[tIndex] = newData;
		}

		/// <summary>
		/// 待機状態か判断して返す。
		/// </summary>
		/// <param name="tPtr">待機処理を行いたいclassのポインタ</param>
		template<class T>
		bool IsWait(const std::shared_ptr<T>& tPtr) 
		{
			auto tIndex = type_index(typeid(tPtr));

			//自分のタイプが設定されていたら
			if (m_params.count(tIndex) != 0) {
				return !m_params[tIndex]->isEnd; //終了状態でないならtrue(待機状態)
			}
			else {
				return false; //登録されてないならそもそも待機しない。
			}
		}

		/// <summary>
		/// 強制的に待機状態を解除する時に使用する関数
		/// </summary>
		/// <param name="tPtr">待機処理を行いたいclassのポインタ</param>
		/// <param name="isEndAction">終了関数を呼び出すかどうか</param>
		template<class T>
		void AbsoluteEndTimer(const std::shared_ptr<T>& tPtr, const bool isEndAction) 
		{
			auto tIndex = type_index(typeid(tPtr));

			if (m_params.count(tIndex) == 0) { //設定されていなかったら
				return;  //処理をしない。
			}

			m_params[tIndex]->EndTimer(isEndAction);
		}

	};

}

//endbasecross