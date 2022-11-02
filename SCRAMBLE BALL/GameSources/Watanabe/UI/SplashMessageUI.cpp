#include "stdafx.h"
#include "SplashMessageUI.h"
#include "../Utility/DataExtracter.h"

namespace basecross {
	// 初期化
	map<SplashMessageUI::MessageType, SplashMessageUI::MessageData> SplashMessageUI::sm_Type2DataMap =
	{
		{MessageType::Relocation,MessageData(SimpleSprite::Type::SpriteData,L"Relocation")},
		{MessageType::GetBall,MessageData(SimpleSprite::Type::SpriteData,L"GetBall")},
		{MessageType::StolenBall,MessageData(SimpleSprite::Type::SpriteData,L"StolenBall")},
		{MessageType::CreateRoom,MessageData(SimpleSprite::Type::SpriteData,L"CreatedRoom")},
		{MessageType::JoinRoom,MessageData(SimpleSprite::Type::SpriteData,L"JoinedRoom")},
		{MessageType::CanNotJoin,MessageData(SimpleSprite::Type::SpriteData,L"CanNotJoin")},
		{MessageType::Remaining60s,MessageData(SimpleSprite::Type::SpriteData,L"Remaining60s")},
		{MessageType::Remaining30s,MessageData(SimpleSprite::Type::SpriteData,L"Remaining30s")},
		{MessageType::Goal,MessageData(SimpleSprite::Type::SpriteData,L"Goal")},
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
		m_sprite = GetStage()->AddGameObject<SimpleSprite>(SimpleSprite::Type::SpriteData, L"StolenBall");
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

	void SplashMessageUI::SetColor(const Col4& color) {
		m_sprite->GetDrawComponent()->SetDiffuse(color);
	}

	Col4 SplashMessageUI::GetColor() {
		return m_sprite->GetDrawComponent()->GetDiffuse();
	}
}