#include "stdafx.h"
#include "HPGaugeUI.h"
#include "../Utility/Utility.h"
#include "../Utility/DataExtracter.h"

namespace basecross {
	HPGaugeUI::HPGaugeUI(const shared_ptr<Stage>& stage)
		:UIObjectBase(stage, L"HPGaugeUI")
	{}

	HPGaugeUI::HPGaugeUI(const shared_ptr<Stage>& stage, const wstring& line)
		: UIObjectBase(stage, L"HPGaugeUI")
	{
		vector<wstring> tokens = DataExtracter::DelimitData(line);
		DataExtracter::RectTransformDataExtraction(tokens, m_rectTransformData);
	}

	void HPGaugeUI::OnCreate() {
		const wstring TextureKey = L"HPGauge_TX";

		Vec2 origin(0.0f);
		auto size = Utility::GetTextureSize(TextureKey);
		// テクスチャ座標でUVを定義
		vector<Vec2> uvs, uv = {
			origin,
			origin + Vec2(size.x,0.0f),
			origin + Vec2(0.0f, size.y),
			origin + size
		};
		// テクスチャ座標からUV座標に変換
		Utility::ConvertToUVCoordinates(uv, TextureKey, uvs);
		Col4 color(1.0f);
		Vec2 halfSize = size / 2.0f;
		// 頂点のデータ (番号は左上から右下まで)
		vector<VertexPositionColorTexture> vertices = {
			{Vec3(-halfSize.x,+halfSize.y,0.0f),color,uvs[0]}, //0
			{Vec3(+halfSize.x,+halfSize.y,0.0f),color,uvs[1]}, //1

			{Vec3(-halfSize.x,-halfSize.y,0.0f),color,uvs[2]}, //2
			{Vec3(+halfSize.x,-halfSize.y,0.0f),color,uvs[3]},  //3
		};
		// 頂点インデックス
		vector<uint16_t> indices = {
			0, 1, 2, // 上の三角形
			2, 1, 3  // 下の三角形
		};

		m_drawComp = AddComponent<PCTGaugeSprite>(vertices, indices);
		auto ptrDraw = m_drawComp.lock();
		ptrDraw->SetTextureResource(TextureKey);
		ptrDraw->SetGaugeColor(Col4(0, 1, 0, 1));
		ptrDraw->IsBackground(true);
		ptrDraw->SetGaugeGradientTexture(L"HPColorGauge_TX");

		SetAlphaActive(true);
	}
	void HPGaugeUI::OnUpdate() {
		if (m_status.use_count() == 0)
			return;

		auto status = m_status.lock()->GetStatus();
		m_drawComp.lock()->SetRate(status.hp / (float)status.maxHp);
	}

	void HPGaugeUI::SetPlayerStatus(const shared_ptr<PlayerStatus> status) {
		m_status = status;
	}
}