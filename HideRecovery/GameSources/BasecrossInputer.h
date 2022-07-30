#pragma once

using namespace itbs::Input;

namespace basecross
{
	/// <summary>
	/// Basecross用登録コントローラー
	/// </summary>
	class BasecrossInputer : public InputerBase
	{
	protected:
		/// <summary>
		/// 継承コンストラクタ
		/// </summary>
		/// <returns>なし</returns>
		BasecrossInputer() noexcept = default;

		/// <summary>
		/// InputDevice取得用関数
		/// </summary>
		/// <returns>InputDeviceポインタ</returns>
		static const std::shared_ptr<itbs::Input::InputDevice> GetInputDevice();
	};
}