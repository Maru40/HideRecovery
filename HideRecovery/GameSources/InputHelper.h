#pragma once
#include<Windows.h>
#include<Xinput.h>
#include<unordered_map>
#include<array>
#include"DelegateSystem.h"
#include<queue>

namespace itbs
{
	namespace Input
	{
		/// <summary>
		/// キーボードのキー列挙型
		/// </summary>
		enum class KeyCode : UINT8
		{
			MouseLeftButton = VK_LBUTTON,
			MouseRightButton = VK_RBUTTON,
			MouseMiddleButton = VK_MBUTTON,

			Backspace = VK_BACK,
			Tab = VK_TAB,
			Clear = VK_CLEAR,
			Enter = VK_RETURN,
			Shift = VK_SHIFT,
			Control = VK_CONTROL,
			Alt = VK_MENU,
			Pause = VK_PAUSE,
			CapsLock = VK_CAPITAL,
			Escape = VK_ESCAPE,
			Space = VK_SPACE,
			PageUp = VK_PRIOR,
			PageDown = VK_NEXT,
			End = VK_END,
			Home = VK_HOME,

			LeftArrow = VK_LEFT,
			UpArrow = VK_UP,
			RightArrow = VK_RIGHT,
			DownArrow = VK_DOWN,

			PrintScreen = VK_SNAPSHOT,
			Insert = VK_INSERT,
			Delete = VK_DELETE,

			Alpha0 = '0',
			Alpha1, Alpha2, Alpha3, Alpha4,
			Alpha5, Alpha6, Alpha7, Alpha8, Alpha9,

			A = 'A',
			B, C, D, E, F, G, H, I, J, K, L, M, N,
			O, P, Q, R, S, T, U, V, W, X, Y, Z,

			NumPad0 = VK_NUMPAD0,
			Numpad1, Numpad2, Numpad3, Numpad4,
			Numpad5, Numpad6, Numpad7, Numpad8, Numpad9,

			Multiply = VK_MULTIPLY,
			Add = VK_ADD,
			Subtract = VK_SUBTRACT,
			Decimal = VK_DECIMAL,
			Divide = VK_DIVIDE,

			F1 = VK_F1,
			F2, F3, F4, F5, F6, F7, F8, F9,
			F10, F11, F12, F13, F14, F15, F16,
			F17, F18, F19, F20, F21, F22, F23, F24,

			NumLock = VK_NUMLOCK,
			Scroll = VK_SCROLL,
			
			LeftShift = VK_LSHIFT,
			RightShift = VK_RSHIFT,
			LeftControll = VK_LCONTROL,
			RightControll = VK_RCONTROL,
			LeftAlt = VK_LMENU,
			RightAlt = VK_RMENU,

			Colon = VK_OEM_1,
			Plus = VK_OEM_PLUS,
			Comma = VK_OEM_COMMA,
			Minus = VK_OEM_MINUS,
			Period = VK_OEM_PERIOD,
			Slash = VK_OEM_2,
			At = VK_OEM_3,
			StartSquareBracket = VK_OEM_4,
			YenSign = VK_OEM_5,
			EndSquareBracket = VK_OEM_6,
			Tilde = VK_OEM_7,
			BackSlash = VK_OEM_102
		};

		/// <summary>
		/// XBoxゲームパッドのボタン列挙型
		/// </summary>
		enum class XInputCode : WORD
		{
			A = XINPUT_GAMEPAD_A,
			B = XINPUT_GAMEPAD_B,
			X = XINPUT_GAMEPAD_X,
			Y = XINPUT_GAMEPAD_Y,

			Start = XINPUT_GAMEPAD_START,
			Back = XINPUT_GAMEPAD_BACK,
			LeftThumb = XINPUT_GAMEPAD_LEFT_THUMB,
			RightThumb = XINPUT_GAMEPAD_RIGHT_THUMB,
			LeftShoulder = XINPUT_GAMEPAD_LEFT_SHOULDER,
			RightShoulder = XINPUT_GAMEPAD_RIGHT_SHOULDER,

			Up = XINPUT_GAMEPAD_DPAD_UP,
			Down = XINPUT_GAMEPAD_DPAD_DOWN,
			left = XINPUT_GAMEPAD_DPAD_LEFT,
			Right = XINPUT_GAMEPAD_DPAD_RIGHT
		};

		class CursorSetting
		{
			static bool m_isLock;
			static bool m_isDraw;

		public:
			static constexpr int defaultPointX = 1 + 2;

			static void SetIsLock(const bool isLock);
			static void SetIsDraw(const bool isDraw);

			static bool GetIsLock();
			static bool GetIsDraw();
		};

		template<class T>
		class I_InputEventGetter
		{
		public:
			virtual bool IsInputDown(const T& key) const = 0;
			virtual bool IsInputPush(const T& key) const = 0;
			virtual bool IsInputUp(const T& key) const = 0;
		};

		/// <summary>
		/// マウスデータ管理クラス
		/// </summary>
		class MouseState
		{
			POINT m_mousePosition;

			POINT m_mouseMoveRange;

			bool m_isCursorDraw = true;

			bool m_isBeforeLock = false;
			
		public:

			void UpdateMouseState(const HWND& hwnd);

			POINT GetMouseMove() const;
		};

		class I_KeyBoardInputEventGetter : public I_InputEventGetter<KeyCode>
		{
		public:

		};

		/// <summary>
		/// キーボード入力データ管理クラス
		/// </summary>
		class KeyBoardState : public I_KeyBoardInputEventGetter
		{
			/// <summary>
			/// キーコード最大数
			/// </summary>
			static constexpr int MAX_KEYCODE = 256;

			/// <summary>
			/// GetAsyncKeyStateで取得したキー情報の押されているかどうかのフラグビット
			/// </summary>
			static constexpr unsigned short GET_ASYNC_KEY_PUSH_BIT = 0x8000;

			/// <summary>
			/// １回呼び出し前の入力されたキーテーブル
			/// </summary>
			std::array<bool, MAX_KEYCODE> m_beforeKeycodeTable;
			/// <summary>
			/// 現在のフレームに入力されたキーテーブル
			/// </summary>
			std::array<bool, MAX_KEYCODE> m_nowKeycodeTable;

			std::queue<KeyCode> m_downEventQueue;

			std::queue<KeyCode> m_upEventQueue;
		public:
			/// <summary>
			/// キーボード入力の更新
			/// </summary>
			void UpdateInputKeyState();

			/// <summary>
			/// キーコードに対応するキーが押された瞬間かを判別する関数
			/// </summary>
			/// <param name="keycode">判別用キーコード</param>
			/// <returns>押された瞬間ならtrue</returns>
			bool IsInputDown(const KeyCode& keycode) const override;

			/// <summary>
			/// キーコードに対応するキーが押されているかを判別する関数
			/// </summary>
			/// <param name="keycode">判別用キーコード</param>
			/// <returns>押されているならtrue</returns>
			bool IsInputPush(const KeyCode& keycode) const override;

			/// <summary>
			/// キーコードに対応するキーが離された瞬間かを判別する関数
			/// </summary>
			/// <param name="keycode">判別用キーコード</param>
			/// <returns>離された瞬間ならtrue</returns>
			bool IsInputUp(const KeyCode& keycode) const override;

			void AddDownEvent(const KeyCode& keycode);

			void AddUpEvent(const KeyCode& keycode);
		};

		/// <summary>
		/// XBoxコントローラー入力情報構造体
		/// </summary>
		struct XInputGamePadThumbState
		{
			static constexpr SHORT INPUT_MAX_VALUE = 0x7FFF;

			float x;
			float y;

			/// <summary>
			/// 入力情報の更新
			/// </summary>
			/// <param name="x">X入力</param>
			/// <param name="y">Y入力</param>
			/// <param name="deadzone">しきい値</param>
			void UpdateState(const float x, const float y,const int deadzone);
		};

		class I_XInputEventGetter : public I_InputEventGetter<XInputCode>
		{
		public:
			/// <summary>
			/// 左スティック入力を取得する
			/// </summary>
			/// <returns>左スティック入力情報</returns>
			virtual const XInputGamePadThumbState& GetLeftThumb() const = 0;

			/// <summary>
			/// 右スティック入力を取得する
			/// </summary>
			/// <returns>右スティック入力情報</returns>
			virtual const XInputGamePadThumbState& GetRightThumb() const = 0;
		};
		/// <summary>
		/// XBoxコントローラー入力データ管理クラス
		/// </summary>
		class XInputGamePadState : public I_XInputEventGetter
		{
			/// <summary>
			/// １回呼び出し前のゲームパッド情報
			/// </summary>
			XINPUT_GAMEPAD m_beforeGamepad;
			/// <summary>
			/// 現在のゲームパッド情報
			/// </summary>
			XINPUT_GAMEPAD m_nowGamepad;

			/// <summary>
			/// 左スティック入力情報
			/// </summary>
			XInputGamePadThumbState m_leftThumState;
			/// <summary>
			/// 右スティック入力情報
			/// </summary>
			XInputGamePadThumbState m_rightThumState;

		public:
			/// <summary>
			///	ゲームパッド入力の更新
			/// </summary>
			/// <param name="padNum">ゲームパッド番号</param>
			void UpdateGamePadState(const int& padNum);

			/// <summary>
			/// コードに対応するボタンが押された瞬間かを判別する関数
			/// </summary>
			/// <param name="xinputCode">判別用コード</param>
			/// <returns>押された瞬間ならtrue</returns>
			bool IsInputDown(const XInputCode& xinputCode) const override;

			/// <summary>
			/// コードに対応するボタンが押されているかを判別する関数
			/// </summary>
			/// <param name="xinputCode">判別用コード</param>
			/// <returns>押されているならtrue</returns>
			bool IsInputPush(const XInputCode& xinputCode) const override;

			/// <summary>
			/// コードに対応するボタンが離された瞬間かを判別する関数
			/// </summary>
			/// <param name="xinputCode">判別用コード</param>
			/// <returns>離された瞬間ならtrue</returns>
			bool IsInputUp(const XInputCode& xinputCode) const override;

			/// <summary>
			/// 左スティック入力を取得する
			/// </summary>
			/// <returns>左スティック入力情報</returns>
			const XInputGamePadThumbState& GetLeftThumb() const override;

			/// <summary>
			/// 右スティック入力を取得する
			/// </summary>
			/// <returns>右スティック入力情報</returns>
			const XInputGamePadThumbState& GetRightThumb() const override;
		};

		/// <summary>
		/// XBoxコントローラーの番号とコードのペア構造体
		/// </summary>
		struct XInputCodePair
		{
			/// <summary>
			/// コントローラー番号
			/// </summary>
			int num;
			/// <summary>
			/// コード
			/// </summary>
			XInputCode xinputCode;

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="num">コントローラー番号</param>
			/// <param name="xinputCode">XBoxコントローラーボタンコード</param>
			XInputCodePair(const int num, const XInputCode& xinputCode);

			bool operator==(const XInputCodePair& codePair) const noexcept;

			bool operator!=(const XInputCodePair& codePair) const noexcept;
		};

		/// <summary>
		/// 入力用コード構造体
		/// </summary>
		struct CodeData
		{
			/// <summary>
			/// キーボードコード配列
			/// </summary>
			std::vector<KeyCode> keycodes;
			/// <summary>
			/// Xboxコントローラーコード（番号付き）配列
			/// </summary>
			std::vector<XInputCodePair> xinputCodePairs;

			/// <summary>
			/// 入力コードが一つも設定されていないか
			/// </summary>
			/// <returns>コードがなければtrue</returns>
			bool IsEmptyCode();
		};

		/// <summary>
		/// 入力イベントデータ構造体
		/// </summary>
		struct InputEventData
		{
			/// <summary>
			/// 呼ばれるイベント
			/// </summary>
			Utility::Delegate<void()> inputEvent;

			/// <summary>
			/// イベント対応コード
			/// </summary>
			CodeData codedata;
		};

		/// <summary>
		/// コントローラークラス
		/// </summary>
		class InputerBase
		{
			friend class InputDevice;

			/// <summary>
			/// 押下イベント配列
			/// </summary>
			static std::vector<InputEventData> m_downEventDatas;
			/// <summary>
			/// 離したイベント配列
			/// </summary>
			static std::vector<InputEventData> m_upEventDatas;

			/// <summary>
			/// キーボード入力イベント
			/// </summary>
			/// <param name="keycode">入力キーコード</param>
			/// <param name="eventDatas">対応イベント配列</param>
			void OnKeyEvent(const KeyCode& keycode, std::vector<InputEventData>& eventDatas);

			/// <summary>
			/// XBoxコントローラー入力イベント
			/// </summary>
			/// <remarks>
			/// イベントデータのコードが空ならばAnyKeyとする
			/// </remarks>
			/// <param name="codePair">入力XBoxコード</param>
			/// <param name="eventDatas">対応イベント配列</param>
			void OnXInputEvent(const XInputCodePair& codePair, std::vector<InputEventData>& eventDatas);

			/// <summary>
			/// キーボードが押された入力イベント
			/// </summary>
			/// <param name="keycode">キーコード</param>
			void OnKeyDown(const KeyCode& keycode);
			/// <summary>
			/// XBoxコントローラーが押された入力イベント
			/// </summary>
			/// <param name="num">コントローラー番号</param>
			/// <param name="xinputCode">XBoxコントローラーボタンコード</param>
			void OnXInputDown(const int num, const XInputCode& xinputCode);

			/// <summary>
			/// キーボードが離された入力イベント
			/// </summary>
			/// <param name="keycode">キーコード</param>
			void OnKeyUp(const KeyCode& keycode);
			/// <summary>
			/// XBoxコントローラーが離された入力イベント
			/// </summary>
			/// <param name="num">コントローラー番号</param>
			/// <param name="xinputCode">ボタンコード</param>
			void OnXInputUp(const int num, const XInputCode& xinputCode);

		protected:
			/// <summary>
			/// 継承用コンストラクタ
			/// </summary>
			/// <returns></returns>
			InputerBase() noexcept = default;

			/// <summary>
			/// 押されたイベント追加関数
			/// </summary>
			/// <param name="eventData">イベントデータ</param>
			static void AddDownEvent(const InputEventData& eventData);

			/// <summary>
			/// 離されたイベント追加関数
			/// </summary>
			/// <param name="eventData">イベントデータ</param>
			static void AddUpEvent(const InputEventData& eventData);
		};

		/// <summary>
		/// デバイス入力管理クラス
		/// </summary>
		class InputDevice
		{
			/// <summary>
			/// マウスの状態変数
			/// </summary>
			MouseState m_mouseState;

			/// <summary>
			/// キーボードの状態変数
			/// </summary>
			KeyBoardState m_keyBoardState;

			/// <summary>
			/// 最大XBoxコントローラー管理数
			/// </summary>
			static constexpr int MAX_GAMEPAD_NUM = 4;

			/// <summary>
			/// XBoxコントローラー4つを持つ配列
			/// </summary>
			std::array<XInputGamePadState, MAX_GAMEPAD_NUM> m_gamePadState;

			/// <summary>
			/// コントローラー配列
			/// </summary>
			std::vector<std::unique_ptr<InputerBase>> m_inputers;

			/// <summary>
			/// XBoxコントローラーの入力イベントを呼ぶ関数
			/// </summary>
			void XInputGamePadInputEvent();
		public:
			/// <summary>
			/// 入力デバイスの更新
			/// </summary>
			/// <param name="hwnd">ウィンドウのハンドル</param>
			void UpdateDevice(const HWND& hwnd);

			/// <summary>
			/// キー押下イベント
			/// </summary>
			/// <param name="vk_keycode">押下した仮想キーコード</param>
			void OnKeyDown(const UINT8 vk_keycode);

			/// <summary>
			/// キーを離したイベント
			/// </summary>
			/// <param name="vk_keycode">離した仮想キーコード</param>
			void OnKeyUp(const UINT8 vk_keycode);

			const MouseState& GetMouseState() const noexcept;

			/// <summary>
			/// キーボードの状態を返す関数
			/// </summary>
			/// <returns>キーボードの状態</returns>
			const I_KeyBoardInputEventGetter& GetKeyBoard() const noexcept;

			/// <summary>
			/// XBoxコントローラーの状態を返す関数
			/// </summary>
			/// <param name="num">コントローラー番号</param>
			/// <returns>コントローラーの状態</returns>
			const I_XInputEventGetter& GetXInputGamePad(const int& num = 0) const noexcept;

			/// <summary>
			/// XBoxコントローラーの状態の配列を返す関数
			/// </summary>
			/// <returns>コントローラーの状態の配列</returns>
			const std::array<XInputGamePadState, MAX_GAMEPAD_NUM>& GetXInputGamePads() const noexcept;

			/// <summary>
			/// 管理するコントローラーを登録する関数
			/// </summary>
			/// <param name="inputer">登録するコントローラー</param>
			void AddInputer(InputerBase* inputer);

			void MessageChecker(const UINT message, const WPARAM wParam, const LPARAM lParam);
		};

		class I_BasicInputer
		{
		public:
			virtual bool IsDesitionDown() const = 0;

			virtual bool IsCancelDown() const = 0;

			virtual bool IsUpDown() const = 0;

			virtual bool IsUp() const = 0;

			virtual bool IsDownDown() const = 0;

			virtual bool IsDown() const = 0;

			virtual bool IsLeftDown() const = 0;

			virtual bool IsLeft() const = 0;

			virtual bool IsRightDown() const = 0;

			virtual bool IsRight() const = 0;
		};
	}
}