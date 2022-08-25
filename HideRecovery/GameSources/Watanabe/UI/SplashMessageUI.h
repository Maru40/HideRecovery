#pragma once
#include "stdafx.h"
#include "UIObjectBase.h"
#include "../Interface/I_Performable.h"

namespace basecross {
	class SplashMessageUI :public UIObjectBase, public I_Performable {
	public:
		SplashMessageUI(const shared_ptr<Stage>& stage);
		SplashMessageUI(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate()override;

		/// <summary>
		/// メッセージをセット
		/// </summary>
		/// <param name="key">キー</param>
		void SetMessage(const wstring& key);

		void Start()override;
		void Reset()override;
	};
}
