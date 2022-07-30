/*!
@file TimeManager.cpp
@brief 時間管理実体
*/

#include "stdafx.h"
#include "TimeManager.h"

namespace basecross
{
	void TimeData::EndTimer()
	{
		if (func) // 関数を持っていたら
		{
			if (func.operator bool()) // 関数が実行可能かを判定
			{
				func(); // 実行
			}
		}

		if (flag != NULL) // フラグを持っていたら
		{
			*flag = !*flag; // 反転
		}
	}

	//------------------------------------------------------------------------------------------------
	void TimeManager::OnUpdate()
	{
		auto iter = m_datas.begin();
		while (iter != m_datas.end()) // 参照先が最後じゃ無ければ
		{
			//残り秒数からデルタ時間を引く
			auto delta = App::GetApp()->GetElapsedTime();
			iter->time -= delta;

			//残り時間が0なら
			if (iter->time <= 0.0f)
			{
				iter->EndTimer(); //関数の実行(またはフラグの反転)

				//タイマーの削除
				iter = m_datas.erase(iter); // 消去する
				continue;
			}
			iter++;
		}
	}

	bool TimeManager::AddTimer(const TimeData& data)
	{
		if (m_datas.size() == 0) // 管理しているタイマーが1つもないなら
		{
			m_datas.push_back(data); // タイマーの追加
			return true;
		}

		for (auto mData : m_datas)
		{
			if (mData.timerName == data.timerName) //タイマーの名前が既にあるなら
			{
				return false; // 追加せずに終了
			}
		}
		m_datas.push_back(data); // 同名のタイマーが無ければ追加
		return true;
	}

	void TimeManager::ReSetTimer(const TimeData& data)
	{
		for (auto mData : m_datas)
		{
			if (mData.timerName == data.timerName) //同じ名前のものがあったら
			{
				mData = data; // データを更新
				return;
			}
		}
		m_datas.push_back(data); //無かったら新たに追加する
	}

	float TimeManager::GetTime(const wstring& str)
	{
		for (auto data : m_datas)
		{
			if (str == data.timerName) // 目的の名前と同じ名前のタイマーを探す
			{
				return data.time; // 見つかったら時間を返す
			}
		}
		return 0.0f; // 見つからなかったら0を返す
	}

	float TimeManager::GetTimeRate(const wstring& str)
	{
		for (auto data : m_datas)
		{
			if (str == data.timerName)  // 目的の名前と同じ名前のタイマーを探す
			{
				return data.time / data.endTime; // 見つかったら値を返す
			}
		}
		return 0.0f; // 見つからなかったら0を返す
	}
};