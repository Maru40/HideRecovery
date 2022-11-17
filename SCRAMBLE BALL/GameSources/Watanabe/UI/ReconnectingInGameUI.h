#pragma once
#include "stdafx.h"
#include "UIObjectBase.h"

namespace basecross {
	class InProcessUI;

	class ReconnectingInGameUI :public UIObjectBase {
	public:
		/// <summary>
		/// UIの状態
		/// </summary>
		enum class State {
			None,			// 通常（なにも表示しない）
			InConnection,	// 接続中（グルグル）
			Abort,			// 接続を断念（タイトルへ）
		};
	private:
		shared_ptr<InProcessUI> m_inProcessUI;
		shared_ptr<UIObjectBase> m_toTitleUI;
		shared_ptr<UIObjectBase> m_cannotConnectUI;
	public:
		ReconnectingInGameUI(const shared_ptr<Stage>& stage);

		void OnCreate()override;

		void SetState(const State& state);
	};
}
