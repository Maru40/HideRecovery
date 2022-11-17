#include "stdafx.h"
#include "ReconnectingInGameUI.h"

#include "UIObjectCSVBuilder.h"

#include "InProcessUI.h"
#include "SimpleSprite.h"
#include "ButtonUI.h"

namespace basecross {
	ReconnectingInGameUI::ReconnectingInGameUI(const shared_ptr<Stage>& stage)
		:UIObjectBase(stage, L"ReconnectingInGameUI")
	{}

	void ReconnectingInGameUI::OnCreate() {
		// UIパーツ生成
		auto uiBuilder = UIObjectCSVBuilder::Create();
		uiBuilder->Register<SimpleSprite>(L"SimpleSprite");
		uiBuilder->Register<ButtonUI>(L"ButtonUI");
		uiBuilder->Register<InProcessUI>(L"InProcessUI");
		auto dir = App::GetApp()->GetDataDirWString();
		auto path = dir + L"CSVDatas/";
		uiBuilder->Build(GetStage(), path + L"ReconnectingInGameUILayout.csv", GetThis<ReconnectingInGameUI>());

		auto inProcessUI = uiBuilder->GetUIObject<InProcessUI>(L"InProcessUI");
		inProcessUI->SetLabel(InProcessUI::LabelType::InReconnection);
		//inProcessUI->SetActive(true);
	}
}