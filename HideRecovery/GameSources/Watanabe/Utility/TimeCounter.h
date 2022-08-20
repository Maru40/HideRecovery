/*!
@file TimeCounter.h
@brief 時間計測クラス
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class TimeCounter {
		// 設定された時間
		float m_intervalTime;
		// 経過時間
		float m_elaspedTime;
		// 設定した時間を経過しているか？
		bool m_isTimeUp;
		// リロードタイマーか
		//（リロードタイマー：最初からm_isTimeUpがtrueの状態のタイマー）
		bool m_isReloadTimer;
	public:
		/**
		 * @brief コンストラクタ
		 *
		 * @param interval 計測する時間
		 * @param isReloadTimer リロードタイマーか
		 */
		TimeCounter(float interval, bool isReloadTimer = false)
			: m_intervalTime(interval), m_elaspedTime(0.0f), m_isTimeUp(false),
			m_isReloadTimer(isReloadTimer)
		{
			if (isReloadTimer)
				m_elaspedTime = m_intervalTime;
		}
		/**
		 * @brief タイマーのリセット
		 */
		void Reset() {
			m_elaspedTime = 0.0f;
			m_isTimeUp = false;
		}

		void ReloadReset() {
			m_elaspedTime = m_intervalTime;
			m_isTimeUp = false;
		}

		/**
		 * @brief タイマーのリセット（新たに計測時間を設定）
		 *
		 * @param interval 計測する時間
		 */
		void Reset(float interval) {
			m_intervalTime = interval;
			m_elaspedTime = 0.0f;
			m_isTimeUp = false;
		}

		/**
		 * @brief カウント
		 *
		 * @return true:経過した/false:経過していない
		 */
		bool Count();

		/**
		 * @brief 設定した時間が経過したか
		 *
		 * @return true:経過した/false:経過していない
		 */
		bool IsTimeUp() {
			return m_isTimeUp;
		}

		/**
		 * @brief 計測時間を設定
		 *
		 * @param interval 計測時間
		 */
		void SetIntervalTime(float interval) {
			SetIntervalTime(interval, false);
		}

		/**
		 * @brief 計測時間を設定
		 *
		 * @param interval 計測時間
		 * @param isReloadTimer リロードタイマーか
		 */
		void SetIntervalTime(float interval, bool isReloadTimer) {
			m_intervalTime = interval;
			if (isReloadTimer)
				m_elaspedTime = m_intervalTime;
		}

		/**
		 * @brief 経過時間 / 設定された時間 の割合を取得
		 *
		 * @return float 経過時間 / 設定された時間 の割合
		 */
		float GetTimeRate() {
			return m_elaspedTime / m_intervalTime;
		}

		/**
		 * @brief (1.0f - 経過時間 / 設定された時間)を取得
		 *
		 * @return float (1.0f - 経過時間 / 設定された時間)
		 */
		float GetInverseTimeRate() {
			return 1 - GetTimeRate();
		}

		/**
		 * @brief 残り時間を取得
		 *
		 * @return float 残り時間
		 */
		float GetLeftTime() {
			return m_intervalTime - m_elaspedTime;
		}

		/**
		 * @brief 経過時間を取得
		 *
		 * @return float 経過時間
		 */
		float GetElaspedTime() {
			return m_elaspedTime;
		}

		/**
		 * @brief 計測時間を取得
		 *
		 * @return 計測時間
		 */
		float GetIntervalTime() {
			return m_intervalTime;
		}
	};
}
//end basecross