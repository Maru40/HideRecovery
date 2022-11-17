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

		// タイトルに戻るUI
		auto toTitle = uiBuilder->GetUIObject(L"ToTitle");
		auto button = uiBuilder->GetUIObject(L"Button");
		// ToTitleを親に
		button->SetParent(toTitle);
		// ボタン表示はToTitleと扱いが変わらないので
		// ToTitleと表示非表示を同じにする
		m_toTitleUI = toTitle;

		// InProcessUI
		auto inProcessUI = uiBuilder->GetUIObject<InProcessUI>(L"InProcessUI");
		inProcessUI->SetLabel(InProcessUI::LabelType::InReconnection);
		m_inProcessUI = inProcessUI;

		// 「インターネットに接続できませんでした」UI
		m_cannotConnectUI = uiBuilder->GetUIObject(L"CannotConnect");

		SetState(State::None);
	}

	void ReconnectingInGameUI::SetState(const State& state) {
		switch (state)
		{
		case State::None:
		{
			shared_ptr<UIObjectBase> uis[] = {
				m_cannotConnectUI,
				m_inProcessUI,
				m_toTitleUI
			};
			for (auto& ui : uis) {
				ui->SetActive(false);
			}
		}
		return;
		case State::InConnection:
			m_cannotConnectUI->SetActive(false);
			m_toTitleUI->SetActive(false);
			m_inProcessUI->SetActive(true);
			return;
		case State::Abort:
			m_cannotConnectUI->SetActive(true);
			m_toTitleUI->SetActive(true);
			m_inProcessUI->SetActive(false);
			return;
		}

		// ここまで来ない
		return;
	}
}