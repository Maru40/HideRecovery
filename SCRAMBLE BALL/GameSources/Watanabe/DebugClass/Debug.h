/*!
@file   Debug.h
@brief  デバッグクラス
*/

#pragma once
#include "stdafx.h"
#include "../Utility/BaseSingleton.h"

namespace basecross {
	class Debug :public BaseSingletonGameObject<Debug> {
	private:
		// ログのデータ本体
		vector<wstring> m_logData;
		// 表示するログの最初のインデックス
		//（このインデックスからm_logDataのmaxまで表示）
		int m_displayLogFirstIndex;
		// StringSpriteコンポーネント
		shared_ptr<StringSprite> m_ssComp;
		const Rect2D<float> m_defaultRect;
		/**
		 * @brief ログを登録
		 *
		 * @param text	テキスト
		 */
		void RegisterLog(const wstring& test);
		/**
		 * @brief 現在の時間を文字列で取得
		 *
		 * @return 現在の時間の文字列
		 */
		wstring GetNowTimeString();
		/**
		 * @brief システムの情報を文字列で取得
		 *
		 * @return システムの情報(文字列)
		 */
		wstring GetSystemInfo();
		/**
		 * @brief 罫線を取得
		 *
		 * @return 1行分の罫線の文字列
		 */
		wstring BorderInsertion();
	private: // 以下設定
		// 表示するログの最大数
		int m_maxLogCount;
		// ログの数を表示するか
		bool m_isDisplayForCount;
		// 有効かどうか
		bool m_isActive;
	public:
		Debug(const shared_ptr<Stage>& stage);
		void OnCreate();
		void OnUpdate();

		/**
		 * @brief ログを出力
		 *
		 * @param text ログとして出す文字列
		 */
		void Log(const wstring& text);
		/**
		 * @brief ログを出力
		 *
		 * @param value ログとして出力する値
		 */
		void Log(int value);
		/**
		 * @brief ログを出力
		 *
		 * @param value ログとして出力する値
		 */
		void Log(float value);
		/**
		 * @brief ログを出力
		 *
		 * @param value ログとして出力する値
		 */
		void Log(const Vec2& value);
		/**
		 * @brief ログを出力
		 *
		 * @param value ログとして出力する値
		 */
		void Log(const Vec3& value);
		/**
		 * @brief ログを出力
		 *
		 * @param value ログとして出力する値
		 */
		void Log(const Vec4& value);

		/**
		 * @brief ログをクリアする
		 */
		void ClearLog();

		/**
		 * @brief フォントサイズを変更する
		 *
		 * @param size フォントサイズ
		 */
		void SetFontSize(float size);

		/**
		 * @brief 背景を表示するか
		 *
		 * @param flg true:表示する / false:表示しない
		 */
		void IsShowBackground(bool flg);
	};
}
//end basecross