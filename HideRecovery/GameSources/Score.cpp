#include "stdafx.h"
#include "Score.h"
#include "Scene.h"

namespace basecross
{
	void Score::OnCreate()
	{
		int time = App::GetApp()->GetScene<Scene>()->GetCountTime();
		time < 1 ? m_score = (int)pow(10, 6) - 1 : m_score = m_scoreBase / time;
		float fontspace = 15.0f;
		std::vector<VertexPositionColorTexture> vertices = {
			{Vec3(0.0f,    0.0f, 0.0f), m_color, Vec2(0.0f / 256.0f,0.0f)},
			{Vec3(256.0f,    0.0f, 0.0f), m_color, Vec2(256.0f / 256.0f, 0.0f)},
			{Vec3(0.0f, -64.0f, 0.0f), m_color, Vec2(0.0f / 256.0f, 64.0f / 64.0f)},
			{Vec3(256.0f, -64.0f, 0.0f), m_color, Vec2(256.0f/256.0f, 64.0f / 64.0f)}
		};
		std::vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3
		};
		auto drawComp = AddComponent<PCTSpriteDraw>(vertices, indices);
		drawComp->SetTextureResource(L"Score");

		SetAlphaActive(true);

		auto transComp = AddComponent<Transform>();
		Vec3 pos(100.0f, 0.0f, 0.0f);
		transComp->SetScale(0.8f, 0.9f, 1.0f);
		transComp->SetPosition(pos);
	}


	void Score::OnUpdate()
	{
		int range = 0;
		int scorerange = m_score;
		for (range = 0; scorerange > 0; range++)
		{
			scorerange /= 10;
		}
		if (range < 1)
		{
			range = 1;
		}

		// 数字部分の初期化
		numbers.resize(range);
		Vec3 pos(100.0f, 0.0f, 0.0f);
		Vec3 offset(205.0f + 30.0f * (8 - range), 5.0f, 0.0f);
		for (auto& number : numbers)
		{
			number = ObjectFactory::Create<Number>(GetStage(), 0);
			auto numberTrans = number->GetComponent<Transform>();
			numberTrans->SetPosition(pos + offset); // SCOREの隣に数字が並ぶようにずらす
			numberTrans->SetScale(0.7f, 0.7f, 1.0f);
			offset += Vec3(30.0f, 0.0f, 0.0f); // 数字の幅の分だけさらにずらす
		}


		int place = static_cast<int>(pow(10, numbers.size() - 1));
		for (auto& number : numbers)
		{
			int value = m_score / place % 10;
			place /= 10;

			number->SetValue(value); // 数字を更新
		}
	}


	void Score::OnDraw()
	{
		GameObject::OnDraw();
		for (auto& number : numbers)
		{
			number->OnDraw();
		}
	}

	void ResultRank::OnCreate()
	{
		float fontspace = 15.0f;
		std::vector<VertexPositionColorTexture> vertices = {
			{Vec3(0.0f,    0.0f, 0.0f), m_color, Vec2(0.0f / 256.0f,0.0f)},
			{Vec3(256.0f,    0.0f, 0.0f), m_color, Vec2(256.0f / 256.0f, 0.0f)},
			{Vec3(0.0f, -64.0f, 0.0f), m_color, Vec2(0.0f / 256.0f, 64.0f / 64.0f)},
			{Vec3(256.0f, -64.0f, 0.0f), m_color, Vec2(256.0f / 256.0f, 64.0f / 64.0f)}
		};
		std::vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3
		};
		auto drawComp = AddComponent<PCTSpriteDraw>(vertices, indices);
		drawComp->SetTextureResource(L"Rank");

		SetAlphaActive(true);

		auto transComp = AddComponent<Transform>();
		Vec3 pos(100.0f, -150.0f, 0.0f);
		transComp->SetScale(0.7f, 0.9f, 1.0f);
		transComp->SetPosition(pos);
	}

	void ResultRank::OnUpdate()
	{
		int time = App::GetApp()->GetScene<Scene>()->GetCountTime();
		time < 1 ? m_score = (int)pow(10, 6) - 1 : m_score = m_scoreBase / time;
		m_score >= 15000 ? m_rankNum = 0 : NULL;
		m_score >= 10000 && m_score < 15000 ? m_rankNum = 1 : NULL;
		m_score >= 5000 && m_score < 10000 ? m_rankNum = 2 : NULL;
		m_score < 5000 ? m_rankNum = 3 : NULL;

		Vec3 pos(200.0f, -150.0f, 0.0f);

		// 数字部分の初期化
		ranknumbers.resize(1);
		Vec3 offset(200.0f, 50.0f, 0.0f);
		for (auto& number : ranknumbers)
		{
			number = ObjectFactory::Create<HideRank>(GetStage(), 0);
			auto numberTrans = number->GetComponent<Transform>();
			numberTrans->SetPosition(pos + offset);
			numberTrans->SetScale(1.3f, 1.5f, 1.0f);
			offset += Vec3(30.0f, 0.0f, 0.0f); // 数字の幅の分だけさらにずらす
		}


		int place = static_cast<int>(pow(10, ranknumbers.size() - 1));
		for (auto& number : ranknumbers)
		{
			int value = m_rankNum / place % 10;
			place /= 10;

			number->SetValue(value); // 数字を更新
		}

	}

	void ResultRank::OnDraw()
	{
		GameObject::OnDraw();
		for (auto& number : ranknumbers)
		{
			number->OnDraw();
		}
	}
}
