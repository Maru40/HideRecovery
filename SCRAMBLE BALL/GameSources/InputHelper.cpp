#include"InputHelper.h"

namespace itbs
{
	namespace Input
	{
		// CursorSetting -------------------------------------
		
		bool CursorSetting::m_isLock = false;
		bool CursorSetting::m_isDraw = true;
		

		void CursorSetting::SetIsLock(const bool isLock)
		{
			m_isLock = isLock;
		}

		void CursorSetting::SetIsDraw(const bool isDraw)
		{
			m_isDraw = isDraw;
		}

		bool CursorSetting::GetIsLock()
		{
			return m_isLock;
		}

		bool CursorSetting::GetIsDraw()
		{
			return m_isDraw;
		}

		// MouseState ----------------------------------------
		
		void MouseState::UpdateMouseState(const HWND& hwnd)
		{
			if (!CursorSetting::GetIsLock() || GetForegroundWindow() != hwnd)
			{
				m_mouseMoveRange = POINT();

				if (!m_isCursorDraw)
				{
					ShowCursor(true);

					m_isCursorDraw = true;

					OutputDebugString(L"権限がなくなり表示されました\n");
				}

				m_isBeforeLock = CursorSetting::GetIsLock();

				return;
			}


			POINT point;
			GetCursorPos(&point);

			if (!m_isBeforeLock)
			{
				m_mousePosition = point;
			}

			m_mouseMoveRange.x = point.x - m_mousePosition.x;
			m_mouseMoveRange.y = -(point.y - m_mousePosition.y);

			Vec2 windowCenterPosition;

			RECT windowRect;
			GetWindowRect(hwnd, &windowRect);

			RECT clientRect;
			GetClientRect(hwnd, &clientRect);

			m_mousePosition.x = clientRect.right / 2 + windowRect.left;
			m_mousePosition.y = clientRect.bottom / 2 + windowRect.top;
			
			//カーソルの表示や固定をできるようにするかどうか
			const bool isLock = false;
			if (isLock) {
				SetCursorPos(m_mousePosition.x, m_mousePosition.y);

				if (CursorSetting::GetIsDraw())
				{
					if (!m_isCursorDraw)
					{
						ShowCursor(true);

						m_isCursorDraw = true;

						OutputDebugString(L"カーソルが表示されました\n");
					}
				}
				else
				{
					if (m_isCursorDraw)
					{
						ShowCursor(false);

						m_isCursorDraw = false;

						OutputDebugString(L"カーソルが消えました\n");
					}
				}
			}

			m_isBeforeLock = CursorSetting::GetIsLock();
		}

		POINT MouseState::GetMouseMove() const
		{
			return m_mouseMoveRange;
		}

		// KeyBoardState -------------------------------------

		void KeyBoardState::UpdateInputKeyState()
		{
			m_beforeKeycodeTable.swap(m_nowKeycodeTable);

			for (int i = 0; i < MAX_KEYCODE; i++)
			{
				m_nowKeycodeTable[i] = (GetAsyncKeyState(i) & GET_ASYNC_KEY_PUSH_BIT) != 0;
			}
		}

		bool KeyBoardState::IsInputDown(const KeyCode& keycode) const
		{
			UINT8 tableCode = static_cast<UINT8>(keycode);
			return m_nowKeycodeTable[tableCode] && !m_beforeKeycodeTable[tableCode];
		}

		bool KeyBoardState::IsInputPush(const KeyCode& keycode) const
		{
			UINT8 tableCode = static_cast<UINT8>(keycode);
			return m_nowKeycodeTable[tableCode];
		}

		bool KeyBoardState::IsInputUp(const KeyCode& keycode) const
		{
			UINT8 tableCode = static_cast<UINT8>(keycode);
			return !m_nowKeycodeTable[tableCode] && m_beforeKeycodeTable[tableCode];
		}

		void KeyBoardState::AddDownEvent(const KeyCode& keycode)
		{
			m_downEventQueue.push(keycode);
		}

		void KeyBoardState::AddUpEvent(const KeyCode& keycode)
		{
			m_upEventQueue.push(keycode);
		}

		// XInputGamePadThumbState ----------------------------

		void XInputGamePadThumbState::UpdateState(const float x,const float y,const int deadzone)
		{
			if ((deadzone > x && x > -deadzone) &&
				(deadzone > y && y > -deadzone))
			{
				this->x = 0;
				this->y = 0;
				return;
			}

			this->x = x / INPUT_MAX_VALUE;
			this->y = y / INPUT_MAX_VALUE;
		}

		// XInputGamePadState --------------------------------

		void XInputGamePadState::UpdateGamePadState(const int& padNum)
		{
			XINPUT_STATE state;
			DWORD result = XInputGetState(padNum, &state);

			m_beforeGamepad = m_nowGamepad;

			// パッド情報の取得に成功したならば
			if (result == ERROR_SUCCESS)
			{
				m_nowGamepad = state.Gamepad;

				m_leftThumState.UpdateState(m_nowGamepad.sThumbLX, m_nowGamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
				m_rightThumState.UpdateState(m_nowGamepad.sThumbRX, m_nowGamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
			}
		}

		bool XInputGamePadState::IsInputDown(const XInputCode& xinputCode) const
		{
			WORD buttonCode = static_cast<WORD>(xinputCode);
			return (m_nowGamepad.wButtons & buttonCode) && !(m_beforeGamepad.wButtons & buttonCode);
		}

		bool XInputGamePadState::IsInputPush(const XInputCode& xinputCode) const
		{
			WORD buttonCode = static_cast<WORD>(xinputCode);
			return m_nowGamepad.wButtons & buttonCode;
		}

		bool XInputGamePadState::IsInputUp(const XInputCode& xinputCode) const
		{
			WORD buttonCode = static_cast<WORD>(xinputCode);
			return !(m_nowGamepad.wButtons & buttonCode) && (m_beforeGamepad.wButtons & buttonCode);
		}

		const XInputGamePadThumbState& XInputGamePadState::GetLeftThumb() const
		{
			return m_leftThumState;
		}

		const XInputGamePadThumbState& XInputGamePadState::GetRightThumb() const
		{
			return m_rightThumState;
		}

		// XInputCodePair --------------------------------------

		XInputCodePair::XInputCodePair(const int num, const XInputCode& xinputCode) :
			num(num),
			xinputCode(xinputCode)
		{
			
		}

		bool XInputCodePair::operator==(const XInputCodePair& codePair) const noexcept
		{
			return num == codePair.num && xinputCode == codePair.xinputCode;
		}

		bool XInputCodePair::operator!=(const XInputCodePair& codePair) const noexcept
		{
			return !(*this == codePair);
		}

		// CodeData --------------------------------------

		bool CodeData::IsEmptyCode()
		{
			return keycodes.empty() && xinputCodePairs.empty();
		}

		// InputerBase -----------------------------------------

		std::vector<InputEventData> InputerBase::m_downEventDatas;
		std::vector<InputEventData> InputerBase::m_upEventDatas;

		void InputerBase::OnKeyEvent(const KeyCode& keycode, std::vector<InputEventData>& eventDatas)
		{
			for (auto& eventData : eventDatas)
			{
				// イベントコードが登録されていなければAnyKeyとして扱う
				if (eventData.codedata.IsEmptyCode())
				{
					eventData.inputEvent();
					continue;
				}

				auto& keycodes = eventData.codedata.keycodes;
				auto result = std::find(keycodes.begin(), keycodes.end(), keycode);
				if (result != keycodes.end())
				{
					eventData.inputEvent();
				}
			}
		}

		void InputerBase::OnXInputEvent(const XInputCodePair& codePair, std::vector<InputEventData>& eventDatas)
		{
			for (auto& eventData : eventDatas)
			{
				// イベントコードが登録されていなければAnyKeyとして扱う
				if (eventData.codedata.IsEmptyCode())
				{
					eventData.inputEvent();
					continue;
				}

				auto& codePairs = eventData.codedata.xinputCodePairs;
				auto result = std::find(codePairs.begin(), codePairs.end(), codePair);
				if (result != codePairs.end())
				{
					eventData.inputEvent();
				}
			}
		}

		void InputerBase::OnKeyDown(const KeyCode& keycode)
		{
			OnKeyEvent(keycode, m_downEventDatas);
		}

		void InputerBase::OnXInputDown(const int num, const XInputCode& xinputCode)
		{
			XInputCodePair pair = { num,xinputCode };
			OnXInputEvent(pair, m_downEventDatas);
		}

		void InputerBase::OnKeyUp(const KeyCode& keycode)
		{
			OnKeyEvent(keycode, m_upEventDatas);
		}

		void InputerBase::OnXInputUp(const int num, const XInputCode& xinputCode)
		{
			XInputCodePair pair = { num,xinputCode };
			OnXInputEvent(pair, m_upEventDatas);
		}

		void InputerBase::AddDownEvent(const InputEventData& eventData)
		{
			m_downEventDatas.push_back(eventData);
		}

		void InputerBase::AddUpEvent(const InputEventData& eventData)
		{
			m_upEventDatas.push_back(eventData);
		}

		// InputDevice -----------------------------------------

		void InputDevice::XInputGamePadInputEvent()
		{
			for (int i = 0; i < MAX_GAMEPAD_NUM; ++i)
			{
				// 1ビットずつずらしながら、ボタンの対応ビットを作成し、それと比較してボタンイベントを調べる

				WORD buttonCode = 1;

				auto& gamePad = m_gamePadState[i];

				while (buttonCode != 0)
				{
					XInputCode xinputCode = static_cast<XInputCode>(buttonCode);
					if (gamePad.IsInputDown(xinputCode))
					{
						for (auto& inputer : m_inputers)
						{
							inputer->OnXInputDown(i, xinputCode);
						}
					}

					if (gamePad.IsInputUp(xinputCode))
					{
						for (auto& inputer : m_inputers)
						{
							inputer->OnXInputUp(i, xinputCode);
						}
					}

					buttonCode <<= 1;
				}
			}
		}

		void InputDevice::UpdateDevice(const HWND& hwnd)
		{
			m_mouseState.UpdateMouseState(hwnd);

			m_keyBoardState.UpdateInputKeyState();

			for (int i = 0; i < MAX_GAMEPAD_NUM; ++i)
			{
				m_gamePadState[i].UpdateGamePadState(i);
			}

			XInputGamePadInputEvent();
		}

		void InputDevice::OnKeyDown(const UINT8 vk_keycode)
		{
			for (auto& inputer : m_inputers)
			{
				inputer->OnKeyDown(static_cast<KeyCode>(vk_keycode));
			}
		}

		void InputDevice::OnKeyUp(const UINT8 vk_keycode)
		{
			for (auto& inputer : m_inputers)
			{
				inputer->OnKeyUp(static_cast<KeyCode>(vk_keycode));
			}
		}

		const MouseState& InputDevice::GetMouseState() const noexcept
		{
			return m_mouseState;
		}

		const I_KeyBoardInputEventGetter& InputDevice::GetKeyBoard() const noexcept
		{
			return m_keyBoardState;
		}

		const I_XInputEventGetter& InputDevice::GetXInputGamePad(const int& num) const noexcept
		{
			return m_gamePadState[num];
		}

		const std::array<XInputGamePadState,InputDevice::MAX_GAMEPAD_NUM>& InputDevice::GetXInputGamePads() const noexcept
		{
			return m_gamePadState;
		}

		void InputDevice::AddInputer(InputerBase* inputer)
		{
			m_inputers.push_back(std::unique_ptr<InputerBase>(inputer));
		}

		void InputDevice::MessageChecker(const UINT message, const WPARAM wParam, const LPARAM lParam)
		{
			switch (message)
			{
			case WM_KEYDOWN:
				break;
			default:
				break;
			}
		}
	}
}