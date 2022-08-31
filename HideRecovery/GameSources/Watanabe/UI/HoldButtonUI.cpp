#include "stdafx.h"
#include "HoldButtonUI.h"
#include "../Utility/Utility.h"
#include "../Utility/DataExtracter.h"

namespace basecross {
	HoldButtonUI::HoldButtonUI(const shared_ptr<Stage>& stage)
		:UIObjectBase(stage, L"HoldButtonUI")
	{}

	HoldButtonUI::HoldButtonUI(const shared_ptr<Stage>& stage, const wstring& line)
		: UIObjectBase(stage, L"HoldButtonUI")
	{
		vector<wstring> tokens = DataExtracter::DelimitData(line);
		DataExtracter::RectTransformDataExtraction(tokens, m_rectTransformData);
	}

	void HoldButtonUI::OnCreate() {
		auto texSize = Utility::GetTextureSize(L"CircleGauge_TX");
		auto size = texSize / 2.0f;
		// テクスチャ座標でUVを定義
		vector<Vec2> uv = {
			Vec2(0,0),
			Vec2(1,0),
			Vec2(0,1),
			Vec2(1,1)
		};
		Col4 color(1.0f);
		// 頂点のデータ (番号は左上から右下まで)
		vector<VertexPositionColorTexture> vertices = {
			{Vec3(-size.x,+size.y,0.0f),color,uv[0]}, //0
			{Vec3(+size.x,+size.y,0.0f),color,uv[1]}, //1

			{Vec3(-size.x,-size.y,0.0f),color,uv[2]}, //2
			{Vec3(+size.x,-size.y,0.0f),color,uv[3]},  //3
		};
		// 頂点インデックス
		vector<uint16_t> indices = {
			0, 1, 2, // 上の三角形
			2, 1, 3  // 下の三角形
		};

		m_drawComp = AddComponent<PCTGaugeSprite>(vertices, indices);
		auto ptrDraw = m_drawComp.lock();
		ptrDraw->SetTextureResource(L"CircleGauge_TX");
		ptrDraw->SetGaugeColor(Col4(0, 1, 0, 1));
		ptrDraw->IsBackground(true);

		SetAlphaActive(true);
	}
	void HoldButtonUI::OnUpdate() {
		auto transitioner = m_transitioner.lock();
		if (transitioner) {
			auto ptrDraw = m_drawComp.lock();
			ptrDraw->SetRate(transitioner->GetHoldTimer().GetTimeRate());
		}
	}
}