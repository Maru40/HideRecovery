#pragma once
#include "stdafx.h"
#include "UIObjectBase.h"
#include "SimpleSprite.h"

namespace basecross {
	class ButtonUI :public UIObjectBase {
		shared_ptr<SimpleSprite> m_buttonBack;
		shared_ptr<SimpleSprite> m_buttonText;
		wstring m_buttonStr;
	public:
		ButtonUI(const shared_ptr<Stage>& stage);
		ButtonUI(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate()override;

		void SetActive(bool flg)override;
		void ChangeButton(const wstring& buttonString);

		void SetChildDrawLayer(int layer);
	};
}
