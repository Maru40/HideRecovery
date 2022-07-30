#include "stdafx.h"
#include "CountTime.h"
#include "Scene.h"
#include "Freeze.h"

namespace basecross
{
	void CountTime::OnCreate()
	{
		float fontspace = 15.0f;
		std::vector<VertexPositionColorTexture> vertices = {
			{Vec3(0.0f,    0.0f, 0.0f), m_color, Vec2(0.0f, 0.0f)},
			{Vec3(916.0f,    0.0f, 0.0f), m_color, Vec2(1.0f, 0.0f)},
			{Vec3(0.0f, -258.0f, 0.0f), m_color, Vec2(0.0f, 1.0f)},
			{Vec3(916.0f, -258.0f, 0.0f), m_color, Vec2(1.0f, 1.0f)}
		};
		std::vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3
		};
		auto drawComp = AddComponent<PCTSpriteDraw>(vertices, indices);
		drawComp->SetTextureResource(L"timeboard");

		SetAlphaActive(true);

		auto transComp = AddComponent<Transform>();
		Vec3 pos(-600.0f, 385.0f, 0.0f);
		transComp->SetScale(0.25f, 0.25f, 1.0f);
		transComp->SetPosition(pos);
		SetDrawLayer(0);
	}

	void CountTime::OnUpdate()
	{
		auto scene = App::GetApp()->GetScene<Scene>();
		Vec3 pos(100.0f, 400.0f, 0.0f);
		float delta = App::GetApp()->GetElapsedTime();
		AddComponent<Freeze>();
		m_count += delta;
		if (m_count > 1.0f && m_time < 5999)
		{
			m_count = 0.0f;
			m_time++;
		}


		// 数字部分の初期化
		numbers.resize(2);
		Vec3 offset(-560.0f, -22.5f, 0.0f);
		for (auto& number : numbers)
		{
			number = ObjectFactory::Create<Number>(GetStage(), 0);
			auto numberTrans = number->GetComponent<Transform>();
			numberTrans->SetPosition(pos + offset); // SCOREの隣に数字が並ぶようにずらす
			numberTrans->SetScale(0.6f, 0.5f, 1.0f);
			offset += Vec3(30.0f, 0.0f, 0.0f); // 数字の幅の分だけさらにずらす
		}


		int place = static_cast<int>(pow(10, numbers.size() - 1));
		for (auto& number : numbers)
		{
			int value = (m_time % 60) / place % 10;
			place /= 10;

			number->SetValue(value); // 数字を更新
		}
		scene->SetCountTime(m_time);
	}

	void CountTime::OnDraw()
	{
		GameObject::OnDraw();
		for (auto& number : numbers)
		{
			number->OnDraw();
		}
	}

	void CountTimeMinute::OnCreate()
	{
		float fontspace = 15.0f;
		std::vector<VertexPositionColorTexture> vertices = {
			{Vec3(0.0f,    0.0f, 0.0f), m_color, Vec2(0.0f, 0.0f)},
			{Vec3(40.0f,    0.0f, 0.0f), m_color, Vec2(1.0f, 0.0f)},
			{Vec3(0.0f, -72.0f, 0.0f), m_color, Vec2(0.0f, 1.0f)},
			{Vec3(40.0f, -72.0f, 0.0f), m_color, Vec2(1.0f, 1.0f)}
		};
		std::vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3
		};
		auto drawComp = AddComponent<PCTSpriteDraw>(vertices, indices);
		drawComp->SetTextureResource(L"colon");

		SetAlphaActive(true);

		auto transComp = AddComponent<Transform>();
		Vec3 pos(-478.0f, 370.0f, 0.0f);
		transComp->SetScale(0.5f, 0.5f, 1.0f);
		transComp->SetPosition(pos);

		SetDrawLayer(1);
	}

	void CountTimeMinute::OnUpdate()
	{
		auto scene = App::GetApp()->GetScene<Scene>();
		m_time = scene->GetCountTime();


		Vec3 pos(30.0f, 400.0f, 0.0f);
		// 数字部分の初期化
		numbers.resize(2);
		Vec3 offset(-565.0f, -22.5f, 0.0f);
		for (auto& number : numbers)
		{
			number = ObjectFactory::Create<Number>(GetStage(), 0);
			auto numberTrans = number->GetComponent<Transform>();
			numberTrans->SetPosition(pos + offset);
			numberTrans->SetScale(0.6f, 0.5f, 1.0f);
			offset += Vec3(30.0f, 0.0f, 0.0f); // 数字の幅の分だけさらにずらす
		}

		
		int place = static_cast<int>(pow(10, numbers.size() - 1));
		for (auto& number : numbers)
		{
			int value = (m_time / 60) / place % 10;
			place /= 10;

			number->SetValue(value); // 数字を更新
		}

	}

	void CountTimeMinute::OnDraw()
	{
		GameObject::OnDraw();
		for (auto& number : numbers)
		{
			number->OnDraw();
		}
	}
}
