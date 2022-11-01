/*!
@file   GameController.cpp
@brief  コントローラー管理クラスの実体
 */

#include "stdafx.h"
#include "GameController.h"
#include "Scene.h"
 //#include "Stage/GameStage.h"

namespace basecross {
	GameController::GameController(UINT number)
		:m_controllerNumber(number)
	{}

	GameController::~GameController() {
		// リセットしないとバイブレーションが止まらない
		ResetVibration();
	}

	bool GameController::IsAcceptInput() {
		//auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		//auto gameStage = dynamic_pointer_cast<GameStage>(stage);

		//// 現在のステージがGameStage
		//if (gameStage) {
		//	// ポーズとかでなければtrueになる
		//	return gameStage->IsGamePlaying();
		//}
		return true;
	}

	CONTROLER_STATE GameController::GetControler() {
		const auto& cntlPadVec = App::GetApp()->GetInputDevice().GetControlerVec();
		// 0以下の場合強制的に0にする
		auto index = (UINT)m_controllerNumber < 0 ? 0 : (UINT)m_controllerNumber;
		CONTROLER_STATE cntlPad = cntlPadVec[index];
		return cntlPad;
	}

	Vec3 GameController::GetLeftStickVec() {
		auto vec = GetStickVec(Direction::Left);
		return Vec3(vec.x, 0.0f, vec.y);
	}
	Vec3 GameController::GetRightStickVec() {
		auto vec = GetStickVec(Direction::Right);
		return Vec3(vec.x, 0.0f, vec.y);
	}

	Vec2 GameController::GetStickVec(Direction direction) {
		const auto& cntlPad = GetControler();
		if (!IsAcceptInput())
			return Vec2(0.0f);

		float fThumbX = 0.0f;
		float fThumbY = 0.0f;
		// Directionに応じたスティックを取得
		switch (direction)
		{
		case Direction::Left:
			fThumbX = cntlPad.fThumbLX;
			fThumbY = cntlPad.fThumbLY;
			break;
		case Direction::Right:
			fThumbX = cntlPad.fThumbRX;
			fThumbY = cntlPad.fThumbRY;
			break;
		}

		Vec2 stickVec(0.0f);
		if (fThumbX != 0.0f || fThumbY != 0.0f) {
			stickVec = Vec2(fThumbX, fThumbY);
		}
		return stickVec;
	}

	bool GameController::GetRightTrigger() {
		return GetTrigger(Direction::Right);
	}
	bool GameController::GetLeftTrigger() {
		return GetTrigger(Direction::Left);
	}

	bool GameController::GetTrigger(Direction direction) {
		const auto& cntlPad = GetControler();
		if (!IsAcceptInput())
			return false;

		// Directionに応じたトリガーを判定
		switch (direction)
		{
		case Direction::Left:
			return cntlPad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
		case Direction::Right:
			return cntlPad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
		}

		// エラー
		return false;
	}

	int GameController::GetButtonConstants(ControllerButton button) {
		switch (button)
		{
		case ControllerButton::A:
			return XINPUT_GAMEPAD_A;
		case ControllerButton::B:
			return XINPUT_GAMEPAD_B;
		case ControllerButton::X:
			return XINPUT_GAMEPAD_X;
		case ControllerButton::Y:
			return XINPUT_GAMEPAD_Y;
		case ControllerButton::DPAD_UP:
			return XINPUT_GAMEPAD_DPAD_UP;
		case ControllerButton::DPAD_DOWN:
			return XINPUT_GAMEPAD_DPAD_DOWN;
		case ControllerButton::DPAD_LEFT:
			return XINPUT_GAMEPAD_DPAD_LEFT;
		case ControllerButton::DPAD_RIGHT:
			return XINPUT_GAMEPAD_DPAD_RIGHT;
		case ControllerButton::START:
			return XINPUT_GAMEPAD_START;
		case ControllerButton::BACK:
			return XINPUT_GAMEPAD_BACK;
		case ControllerButton::LEFT_THUMB:
			return XINPUT_GAMEPAD_LEFT_THUMB;
		case ControllerButton::RIGHT_THUMB:
			return XINPUT_GAMEPAD_RIGHT_THUMB;
		case ControllerButton::LEFT_SHOULDER:
			return XINPUT_GAMEPAD_LEFT_SHOULDER;
		case ControllerButton::RIGHT_SHOULDER:
			return XINPUT_GAMEPAD_RIGHT_SHOULDER;
		}
		return -1;
	}

	bool GameController::GetButtons(ControllerButton button) {
		if (!IsAcceptInput())
			return false;
		return GetControler().wButtons & GetButtonConstants(button);
	}
	bool GameController::GetNowUpdateButtons(ControllerButton button) {
		if (!IsAcceptInput())
			return false;
		return GetControler().wNowUpdateButtons & GetButtonConstants(button);
	}
	bool GameController::GetPressedButtons(ControllerButton button) {
		if (!IsAcceptInput())
			return false;
		return GetControler().wPressedButtons & GetButtonConstants(button);
	}
	bool GameController::GetReleasedButtons(ControllerButton button) {
		if (!IsAcceptInput())
			return false;
		return GetControler().wReleasedButtons & GetButtonConstants(button);
	}
	bool GameController::GetLastButtons(ControllerButton button) {
		if (!IsAcceptInput())
			return false;
		return GetControler().wLastButtons & GetButtonConstants(button);
	}

	void GameController::ActiveVibrationThread(const wstring& key, const VibrationData& data) {
		// バイブレーション開始
		XINPUT_VIBRATION vibration = { data.LeftMotorSpeed,data.RightMotorSpeed };
		XInputSetState((WORD)m_controllerNumber, &vibration);
		// 指定した秒数待機
		Sleep(DWORD(data.Time * 1000));
		// バイブレーション停止
		StopVibration(key);
	}

	void GameController::SetVibration(const wstring& key, const VibrationData& data) {
		// すでに同じキーでバイブレーションがONになっている場合何もしない
		if (m_vibrationMap[key])
			return;
		// バイブレーションがONになっている
		m_vibrationMap[key] = true;

		// 無限の場合はスレッドを立てない
		if (data.Time == INFINITY) {
			XINPUT_VIBRATION vibration = { data.LeftMotorSpeed,data.RightMotorSpeed };
			XInputSetState((WORD)m_controllerNumber, &vibration);
			return;
		}

		//他のリソースを読み込むスレッドのスタート
		thread vibrationThread(&GameController::ActiveVibrationThread, this, key, data);
		//終了までは待たない
		vibrationThread.detach();
	}

	void GameController::StopVibration(const wstring& key) {
		// バイブレーションがOFFになっている
		m_vibrationMap[key] = false;

		for (auto vibration : m_vibrationMap) {
			// もしほかにONになっているキーがある場合
			if (vibration.second) {
				// 停止しない
				return;
			}
		}

		// このkey以外のバイブレーションがすべてOFFになっている場合は停止する
		ResetVibration();
	}

	void GameController::ResetVibration() {
		XINPUT_VIBRATION vibration = { 0,0 };
		XInputSetState((WORD)m_controllerNumber, &vibration);
	}
}