/*!
@file DebugObject.cpp
@brief DebugObject‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"
#include "DebugObject.h"

namespace basecross {

	wstringstream DebugObject::sm_wss;
	bool DebugObject::sm_isResetDelta = false;
	
	void DebugObject::OnCreate() {
		auto sprite = AddComponent<StringSprite>();
		//sprite->SetTextRect(Rect2D<float>(10.0f,10.0f,300.0f,300.0f));
		sprite->SetTextRect(Rect2D<float>(10.0f, 400.0f, 400.0f, 750.0f));
		sprite->SetBackColor(Col4(0.5f, 0.5f, 0.5f, 0.5f));
	}

	void DebugObject::OnUpdate() {
		auto sprite = GetComponent<StringSprite>();

		sprite->AddText(sm_wss.str());
		sm_wss.str(L"");
	}

	void DebugObject::OnUpdate2() {
		auto& key = App::GetApp()->GetMyInputDevice()->GetKeyBoard();
		if (key.IsInputDown(itbs::Input::KeyCode::R)) {
			auto sprite = GetComponent<StringSprite>();
			sprite->SetText(L"");
		}

		if (sm_isResetDelta) {
			auto sprite = GetComponent<StringSprite>();
			sprite->SetText(L"");
		}
	}
}