/*!
@file   InProcessUI.cpp
@brief  処理中を示すUIの実体
*/

#include "stdafx.h"
#include "InProcessUI.h"
#include "../Utility/DataExtracter.h"

namespace basecross {
	map<InProcessUI::LabelType, Message::MessageData> InProcessUI::sm_Type2DataMap =
	{
		{LabelType::Matching      ,Message::MessageData(Type::SpriteData,L"Matching")},
		{LabelType::Busy          ,Message::MessageData(Type::SpriteData,L"Busy")},
		{LabelType::InReconnection,Message::MessageData(Type::SpriteData,L"InReconnection")}
	};

	InProcessUI::InProcessUI(const shared_ptr<Stage>& stage)
		:UIObjectBase(stage, L"InProcessUI")
	{}

	InProcessUI::InProcessUI(const shared_ptr<Stage>& stage, const wstring& line)
		: UIObjectBase(stage, L"InProcessUI")
	{
		vector<wstring> tokens = DataExtracter::DelimitData(line);
		DataExtracter::RectTransformDataExtraction(tokens, m_rectTransformData);
	}

	void InProcessUI::OnCreate() {
		m_shape = GetStage()->AddGameObject<SimpleSprite>(Type::Texture, L"Process_TX");
		m_label = GetStage()->AddGameObject<SimpleSprite>();

		m_shape->SetParent(GetThis<InProcessUI>());
		m_label->SetParent(GetThis<InProcessUI>());

		m_shapeRectTrans = m_shape->GetRectTransform();
		SetActive(false);
	}

	void InProcessUI::OnUpdate() {
		// 描画していないときは何もしない
		// ※SetActiveで表示非表示を設定しているため
		// 　実際はなくても動作する（多分）
		if (!GetDrawActive())
			return;

		// 念のため0に
		if (m_totalDelta > 1.0f)
			m_totalDelta = 0.0f;

		auto angle = Lerp::CalculateLerp(0.0f, XM_PIDIV2, 0, 1, m_totalDelta, Lerp::rate::Cube);
		m_shapeRectTrans.lock()->SetRotation(angle);

		m_totalDelta += App::GetApp()->GetElapsedTime();
	}

	void InProcessUI::SetLabel(LabelType messageType) {
		auto data = sm_Type2DataMap[messageType];
		m_label->ChangeSprite(data.Type, data.Key);
	}
}