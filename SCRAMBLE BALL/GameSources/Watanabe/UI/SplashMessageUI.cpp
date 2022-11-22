#include "stdafx.h"
#include "SplashMessageUI.h"
#include "../Utility/DataExtracter.h"

namespace basecross {
	// 初期化
	map<SplashMessageUI::MessageType, Message::MessageData> SplashMessageUI::sm_Type2DataMap =
	{
		{MessageType::Relocation         ,Message::MessageData(Type::SpriteData,L"Relocation")},
		{MessageType::GetBall            ,Message::MessageData(Type::SpriteData,L"GetBall")},
		{MessageType::StolenBall         ,Message::MessageData(Type::SpriteData,L"StolenBall")},
		{MessageType::CreateRoom         ,Message::MessageData(Type::SpriteData,L"CreatedRoom")},
		{MessageType::JoinRoom           ,Message::MessageData(Type::SpriteData,L"JoinedRoom")},
		{MessageType::CanNotJoin         ,Message::MessageData(Type::SpriteData,L"CanNotJoin")},
		{MessageType::Remaining60s       ,Message::MessageData(Type::SpriteData,L"Remaining60s")},
		{MessageType::Remaining30s       ,Message::MessageData(Type::SpriteData,L"Remaining30s")},
		{MessageType::Goal               ,Message::MessageData(Type::SpriteData,L"Goal")},
		{MessageType::CannotConnect      ,Message::MessageData(Type::SpriteData,L"CannotConnect")},
		{MessageType::CannotConnectToHost,Message::MessageData(Type::SpriteData,L"CannotConnectToHost")}
	};

	SplashMessageUI::SplashMessageUI(const shared_ptr<Stage>& stage)
		:UIObjectBase(stage, L"SplashMessageUI"), m_isStart(false), m_timer(3)
	{}

	SplashMessageUI::SplashMessageUI(const shared_ptr<Stage>& stage, const wstring& line)
		: UIObjectBase(stage, L"SplashMessageUI"), m_isStart(false), m_timer(3)
	{
		vector<wstring> tokens = DataExtracter::DelimitData(line);
		size_t nextIndex = DataExtracter::RectTransformDataExtraction(tokens, m_rectTransformData);
		m_timer.SetIntervalTime(stof(tokens[nextIndex]));
	}

	void SplashMessageUI::OnCreate() {
		m_sprite = GetStage()->AddGameObject<SimpleSprite>();
		m_sprite->SetParent(GetThis<SplashMessageUI>());
		m_sprite->SetActive(false);
		m_defaultColor = GetColor();
	}

	void SplashMessageUI::OnUpdate() {
		if (!m_isStart)
			return;

		if (m_timer.Count()) {
			m_isStart = false;
			m_sprite->SetActive(false);
		}
	}

	void SplashMessageUI::SetMessage(MessageType type) {
		auto data = sm_Type2DataMap[type];
		m_sprite->ChangeSprite(data.Type, data.Key);
		m_sprite->SetActive(true);
		m_timer.Reset();
		m_isStart = true;
		SetColor(m_defaultColor);
	}

	void SplashMessageUI::ClearMessage() {
		m_isStart = false;
		m_sprite->SetActive(false);
	}

	void SplashMessageUI::SetColor(const Col4& color) {
		m_sprite->GetDrawComponent()->SetDiffuse(color);
	}

	Col4 SplashMessageUI::GetColor() {
		return m_sprite->GetDrawComponent()->GetDiffuse();
	}
}