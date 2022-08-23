#pragma once
#include "stdafx.h"
#include "UIObjectBase.h"
#include "../Manager/SpriteDataManager.h"

namespace basecross {
	class NumberSprite :public UIObjectBase {
		int m_number;
		vector<VertexPositionColorTexture> vertices;
		weak_ptr<PCTSpriteDraw> m_spriteDraw;
	public:
		NumberSprite(const shared_ptr<Stage>& stage);

		void OnCreate()override;

		void SetValue(int number);
		shared_ptr<PCTSpriteDraw> GetDrawComponent();
	};

	class Numbers :public UIObjectBase {
		// 一桁分の数字オブジェクトの配列
		vector<shared_ptr<NumberSprite>> m_numbers;
		// 桁数
		int m_numDigits;
		// スプライトデータ
		sdm::SpriteData m_spriteData;
		// 1フレーム前のスケール
		Vec2 m_beforeScale;
	public:
		Numbers(const shared_ptr<Stage>& stage, int digits);

		void OnCreate()override;
		void OnUpdate()override;
		void SetNumber(int num);
	};
}
