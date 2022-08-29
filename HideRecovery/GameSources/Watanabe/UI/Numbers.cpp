#include "stdafx.h"
#include "Numbers.h"
#include "../Utility/Utility.h"
#include "../DebugClass/Debug.h"
#include "../Utility/DataExtracter.h"

namespace basecross {
	NumberSprite::NumberSprite(const shared_ptr<Stage>& stage)
		:UIObjectBase(stage, L"NumberSprite"), m_number(0)
	{}

	void NumberSprite::OnCreate() {
		auto data = SpriteDataManager::GetInstance()->GetSpriteData(L"Numbers");

		Vec2 size = data.size;
		float fnumber = static_cast<float>(m_number);
		vector<Vec2> uv01, uv = {
			Vec2((fnumber + 0) * size.x,0.0f),
			Vec2((fnumber + 1) * size.x,0.0f),
			Vec2((fnumber + 0) * size.x,size.y),
			Vec2((fnumber + 1) * size.x,size.y)
		};
		Utility::ConvertToUVCoordinates(uv, data.useTextureKey, uv01);
		auto halfSize = size / 2.0f;
		Col4 color(1);
		vertices = {
			{Vec3(-halfSize.x, +halfSize.y, 0.0f), color, uv01[0]},
			{Vec3(+halfSize.x, +halfSize.y, 0.0f), color, uv01[1]},
			{Vec3(-halfSize.x, -halfSize.y, 0.0f), color, uv01[2]},
			{Vec3(+halfSize.x, -halfSize.y, 0.0f), color, uv01[3]}
		};
		vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3
		};
		m_spriteDraw = AddComponent<PCTSpriteDraw>(vertices, indices);
		m_spriteDraw.lock()->SetTextureResource(data.useTextureKey);

		SetAlphaActive(true);
	}

	void NumberSprite::SetValue(int number) {
		m_number = number;
		sdm::SpriteData data;
		vector<Vec2> uv01, uv;
		// 単体で10を表示
		if (number == 10) {
			data = SpriteDataManager::GetInstance()->GetSpriteData(L"10");
			Vec2 size = data.size;
			uv = {
				data.origin + Vec2(0.0f, 0.0f),
				data.origin + Vec2(size.x, 0.0f),
				data.origin + Vec2(0.0f, size.y),
				data.origin + size
			};
			Utility::ConvertToUVCoordinates(uv, data.useTextureKey, uv01);
			auto halfSize = size / 2.0f;
			Col4 color(1);
			vertices = {
				{Vec3(-halfSize.x, +halfSize.y, 0.0f), color, uv01[0]},
				{Vec3(+halfSize.x, +halfSize.y, 0.0f), color, uv01[1]},
				{Vec3(-halfSize.x, -halfSize.y, 0.0f), color, uv01[2]},
				{Vec3(+halfSize.x, -halfSize.y, 0.0f), color, uv01[3]}
			};
		}
		else {
			data = SpriteDataManager::GetInstance()->GetSpriteData(L"Numbers");
			Vec2 size = data.size;
			float fnumber = static_cast<float>(number);
			uv = {
				Vec2((fnumber + 0) * size.x,0.0f),
				Vec2((fnumber + 1) * size.x,0.0f),
				Vec2((fnumber + 0) * size.x,size.y),
				Vec2((fnumber + 1) * size.x,size.y)
			};
			Utility::ConvertToUVCoordinates(uv, data.useTextureKey, uv01);
			auto halfSize = size / 2.0f;
			Col4 color(1);
			vertices = {
				{Vec3(-halfSize.x, +halfSize.y, 0.0f), color, uv01[0]},
				{Vec3(+halfSize.x, +halfSize.y, 0.0f), color, uv01[1]},
				{Vec3(-halfSize.x, -halfSize.y, 0.0f), color, uv01[2]},
				{Vec3(+halfSize.x, -halfSize.y, 0.0f), color, uv01[3]}
			};
		}
		m_spriteDraw.lock()->UpdateVertices(vertices);
	}

	shared_ptr<PCTSpriteDraw> NumberSprite::GetDrawComponent() {
		return m_spriteDraw.lock();
	}

	Numbers::Numbers(const shared_ptr<Stage>& stage, int digits)
		:UIObjectBase(stage, L"Numbers"), m_numDigits(digits), m_beforeScale(Vec2(0))
	{}

	Numbers::Numbers(const shared_ptr<Stage>& stage, const wstring& line)
		: UIObjectBase(stage, L"Numbers"), m_beforeScale(Vec2(0))
	{
		vector<wstring> tokens = DataExtracter::DelimitData(line);
		size_t nextIndex = DataExtracter::RectTransformDataExtraction(tokens, m_rectTransformData);
		m_numDigits = stoi(tokens[nextIndex]);
	}

	void Numbers::OnCreate() {
		m_spriteData = SpriteDataManager::GetInstance()->GetSpriteData(L"Numbers");
		Vec2 offset(-(m_spriteData.size.x / 2) * (m_numDigits - 1), 0.0f);
		for (int i = 0; i < m_numDigits; i++) {
			auto number = GetStage()->AddGameObject<NumberSprite>();
			number->SetParent(GetThis<Numbers>());
			auto numberRect = number->GetComponent<RectTransform>();
			numberRect->SetPosition(m_rectTransform->GetPosition() + offset);
			m_numbers.push_back(number);

			offset += Vec2(m_spriteData.size.x, 0.0f);
		}
		m_beforeScale = m_rectTransform->GetWorldScale();
	}

	void Numbers::OnUpdate() {
		Vec2 scale = m_rectTransform->GetWorldScale();
		// スケール変更時のみ実行
		if (scale != m_beforeScale) {
			float scaleX = scale.x;
			Vec2 offset(-(m_spriteData.size.x / 2) * (m_numDigits - 1) * scaleX, 0.0f);
			for (auto number : m_numbers) {
				auto numberRect = number->GetComponent<RectTransform>();
				numberRect->SetPosition(m_rectTransform->GetPosition() + offset);

				offset += Vec2(m_spriteData.size.x * scaleX, 0.0f);
			}
		}
		m_beforeScale = m_rectTransform->GetWorldScale();
	}

	void Numbers::SetNumber(int num) {
		if (num < 0)
			return;

		int place = static_cast<int>(pow(10, m_numbers.size() - 1));
		for (auto& number : m_numbers) {
			int value = num / place % 10;
			place /= 10;
			number->SetValue(value); // 数字を更新
		}
	}

	void Numbers::SetColor(const Col4& color) {
		for (auto& number : m_numbers) {
			number->GetDrawComponent()->SetDiffuse(color);
		}
	}
}