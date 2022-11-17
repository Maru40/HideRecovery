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

	void ReconnectingInGameUI::CreateBackgroundSprite() {
		vector<VertexPositionColor> vertices;
		float windowWidth = (float)App::GetApp()->GetGameWidth();
		float windowHeight = (float)App::GetApp()->GetGameHeight();
		Vec2 size(windowWidth, windowHeight);

		auto halfSize = size / 2.0f;
		Col4 color(1);
		vertices = {
			{Vec3(-halfSize.x, +halfSize.y, 0.0f), color},
			{Vec3(+halfSize.x, +halfSize.y, 0.0f), color},
			{Vec3(-halfSize.x, -halfSize.y, 0.0f), color},
			{Vec3(+halfSize.x, -halfSize.y, 0.0f), color}
		};
		vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3
		};

		m_backgroundSprite = GetStage()->AddGameObject<UIObjectBase>(L"BackgroundSprite");
		auto spriteDraw = m_backgroundSprite->AddComponent<PCSpriteDraw>(vertices, indices);
		spriteDraw->SetDiffuse(Col4(0, 0, 0, 0.5f));
		m_backgroundSprite->SetAlphaActive(true);
	}

	void ReconnectingInGameUI::OnCreate() {
		CreateBackgroundSprite();

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

		// 最前面に表示させる
		int baseLayer = 3;
		m_backgroundSprite->SetUIDrawLayer(baseLayer);
		for (auto& ui : uiBuilder->GetUIObjects()) {
			ui->SetUIDrawLayer(baseLayer + 1);
		}

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
				m_toTitleUI,
				m_backgroundSprite
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
			m_backgroundSprite->SetActive(true);
			return;
		case State::Abort:
			m_cannotConnectUI->SetActive(true);
			m_toTitleUI->SetActive(true);
			m_inProcessUI->SetActive(false);
			m_backgroundSprite->SetActive(true);
			return;
		}

		// ここまで来ない
		return;
	}
}