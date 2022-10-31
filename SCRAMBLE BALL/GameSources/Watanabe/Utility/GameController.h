/*!
@file   GameController.h
@brief  コントローラー管理クラス
 */

#pragma once
#include "stdafx.h"
#include "Utility.h"

namespace basecross {
	/**
	 * @brief バイブレーションデータ
	 */
	struct VibrationData
	{
		float Time;				// 継続時間
		WORD RightMotorSpeed;	// 右モーター（0～65535）
		WORD LeftMotorSpeed;	// 左モーター（0～65535）

		/**
		 * @brief コンストラクタ
		 *
		 * @param rightMotorSpeedRate	// 右モーターの回転速度（0～1）(高回転)
		 * @param leftMotorSpeedRate	// 左モーターの回転速度（0～1）(低回転)
		 * @param time					// 継続時間
		 */
		VibrationData(float rightMotorSpeedRate, float leftMotorSpeedRate, float time) {
			this->RightMotorSpeed = WORD(65535 * Utility::Clamp(rightMotorSpeedRate, 0.0f, 1.0f));
			this->LeftMotorSpeed = WORD(65535 * Utility::Clamp(leftMotorSpeedRate, 0.0f, 1.0f));
			this->Time = time;
		}
		/**
		 * @brief コンストラクタ
		 *
		 * @param rightMotorSpeedRate	// 右モーターの回転速度（0～1）(高回転)
		 * @param leftMotorSpeedRate	// 左モーターの回転速度（0～1）(低回転)
		 */
		VibrationData(float rightMotorSpeedRate, float leftMotorSpeedRate) {
			this->RightMotorSpeed = WORD(65535 * Utility::Clamp(rightMotorSpeedRate, 0.0f, 1.0f));
			this->LeftMotorSpeed = WORD(65535 * Utility::Clamp(leftMotorSpeedRate, 0.0f, 1.0f));
			this->Time = INFINITY;
		}
	};

	/**
	 * @brief コントローラーのボタン
	 */
	enum class ControllerButton {
		A, B, X, Y,
		DPAD_UP, DPAD_DOWN, DPAD_LEFT, DPAD_RIGHT,
		START, BACK,
		LEFT_THUMB, RIGHT_THUMB,
		LEFT_SHOULDER, RIGHT_SHOULDER
	};

	class GameController {
		/**
		 * @brief スティックの種類
		 */
		enum class Direction {
			Left,
			Right
		};

		// キーとバイブレーション動作フラグのマップ
		// 例：停止するAの処理と起動するBの処理があるとき
		// BのあとにAを実行するとBが想定通りの動作をしないのを防ぐ
		map<wstring, bool> m_vibrationMap;

		// 取得するコントローラー
		UINT m_controllerNumber;

		/**
		 * @brief スティックを取得
		 *
		 * @param direction 取得したいスティック
		 * @return スティックのデータ（Vec2）
		 */
		Vec2 GetStickVec(Direction direction);
		/**
		 * @brief トリガーを取得
		 *
		 * @param direction 取得したいトリガー
		 * @return トリガーのデータ（bool）
		 */
		bool GetTrigger(Direction direction);

		/**
		 * @brief ControllerButtonを定数に変換
		 *
		 * @param button ボタンの種類
		 * @return XINPUT_GAMEPADの定数
		 */
		int GetButtonConstants(ControllerButton button);

		/**
		 * @brief バイブレーションのアクティブ（スレッド用関数）
		 *
		 * @param key バイブレーションを識別するためのキー
		 * @param data バイブレーションデータ
		 */
		void ActiveVibrationThread(const wstring& key, const VibrationData& data);

		/**
		 * @brief 入力を受け付けるか
		 *
		 * @return trueなら受け付ける
		 */
		bool IsAcceptInput();
	public:
		/**
		 * @brief コンストラクタ
		 *
		 * @param number プレイヤー番号
		 */
		GameController(UINT number);
		/**
		 * @brief デストラクタ
		 */
		~GameController();

		/**
		 * @brief コントローラー取得
		 *
		 * @return CONTROLER_STATE
		 */
		CONTROLER_STATE GetControler();

		/**
		 * @brief 左スティックの取得
		 *
		 * @return Vec3のベクトル
		 */
		Vec3 GetLeftStickVec();

		/**
		 * @brief 右スティックの取得
		 *
		 * @return Vec3のベクトル
		 */
		Vec3 GetRightStickVec();

		/**
		 * @brief 右トリガーが押されているか
		 *
		 * @return trueなら押されている
		 */
		bool GetRightTrigger();
		/**
		 * @brief 左トリガーが押されているか
		 *
		 * @return trueなら押されている
		 */
		bool GetLeftTrigger();

		/**
		 * @brief ボタンの取得
		 *
		 * @param button 判定するボタン
		 * @return bool
		 */
		bool GetButtons(ControllerButton button);
		/**
		 * @brief 状態が変わったかどうかの判定をする
		 *
		 * @param button 判定するボタン
		 * @return bool
		 */
		bool GetNowUpdateButtons(ControllerButton button);
		/**
		 * @brief 押された瞬間を判定する
		 *
		 * @param button 判定するボタン
		 * @return bool
		 */
		bool GetPressedButtons(ControllerButton button);
		/**
		 * @brief 離された瞬間を判定する
		 *
		 * @param button 判定するボタン
		 * @return bool
		 */
		bool GetReleasedButtons(ControllerButton button);
		/**
		 * @brief 一つ前のボタンの取得
		 *
		 * @param button 判定するボタン
		 * @return bool
		 */
		bool GetLastButtons(ControllerButton button);

		/**
		 * @brief バイブレーションのセット
		 *
		 * @param key バイブレーションを識別するためのキー
		 * @param data バイブレーションデータ
		 */
		void SetVibration(const wstring& key, const VibrationData& data);

		/**
		 * @brief バイブレーションの停止
		 *
		 * @param key バイブレーションを識別するためのキー
		 */
		void StopVibration(const wstring& key);

		/**
		 * @brief バイブレーションのリセット
		 */
		void ResetVibration();
	};
}
