#include "stdafx.h"
#include "ResultTime.h"
#include "Scene.h"

namespace basecross
{
	void ResultTime::OnCreate()
	{
		m_time = App::GetApp()->GetScene<Scene>()->GetCountTime();
		float fontspace = 15.0f;
		std::vector<VertexPositionColorTexture> vertices = {
			{Vec3(0.0f,    0.0f, 0.0f), m_color, Vec2(0.0f,            0.0f)},
			{Vec3(174.0f,    0.0f, 0.0f), m_color, Vec2(1.0f,            0.0f)},
			{Vec3(0.0f, -56.0f, 0.0f), m_color, Vec2(0.0f, 1.0f)},
			{Vec3(174.0f, -56.0f, 0.0f), m_color, Vec2(1.0f, 1.0f)}
		};
		std::vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3
		};
		auto drawComp = AddComponent<PCTSpriteDraw>(vertices, indices);
		drawComp->SetTextureResource(L"time");

		SetAlphaActive(true);

		auto transComp = AddComponent<Transform>();
		Vec3 pos(100.0f, 100.0f, 0.0f);
		transComp->SetScale(1.0f, 1.0f, 1.0f);
		transComp->SetPosition(pos);

	}

	void ResultTime::OnUpdate()
	{
		Vec3 pos(100.0f, 100.0f, 0.0f);

		// 数字部分の初期化
		numbers.resize(2);
		Vec3 offset(388.0f, 5.0f, 0.0f);
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
			int value = (m_time % 60) / place % 10;
			place /= 10;

			number->SetValue(value); // 数字を更新
		}

	}

	void ResultTime::OnDraw()
	{
		GameObject::OnDraw();
		for (auto& number : numbers)
		{
			number->OnDraw();
		}
	}

	void ResultTimeMinute::OnCreate()
	{
		m_time = App::GetApp()->GetScene<Scene>()->GetCountTime();
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
		Vec3 pos(460.0f, 95.0f, 0.0f);
		transComp->SetScale(0.7f, 0.7f, 1.0f);
		transComp->SetPosition(pos);

	}

	void ResultTimeMinute::OnUpdate()
	{
		auto scene = App::GetApp()->GetScene<Scene>();
		m_time = scene->GetCountTime();


		Vec3 pos(30.0f, 100.0f, 0.0f);
		// 数字部分の初期化
		numbers.resize(2);
		Vec3 offset(367.0f, 5.0f, 0.0f);
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
			int value = (m_time / 60) / place % 10;
			place /= 10;

			number->SetValue(value); // 数字を更新
		}

	}

	void ResultTimeMinute::OnDraw()
	{
		GameObject::OnDraw();
		for (auto& number : numbers)
		{
			number->OnDraw();
		}
	}

}