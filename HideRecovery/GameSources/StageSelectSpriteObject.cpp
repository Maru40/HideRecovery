/*!
@file StageSelectSpriteObject.cpp
@brief StageSelectSpriteObjectのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "StageSelectSpriteObject.h"

#include "SpriteObject.h"

#include "BuilderVertexPCT.h"
#include "SelectSprite.h"

#include "Number.h"

#include "MoveUI.h"

#include "SinScaling.h"

namespace basecross {

	StageSelectSpriteObject::StageSelectSpriteObject(const std::shared_ptr<Stage>& stage, const uint32_t num, const wstring& stageTexture)
		:GameObject(stage), m_num(num), m_stageTexture(stageTexture)
	{}

	void StageSelectSpriteObject::OnCreate() {
		auto select = AddComponent<SelectSprite>();
		select->AddSelectAction(true, [&]() { m_selectBack->SetDrawActive(true); });
		select->AddSelectAction(false, [&]() { m_selectBack->SetDrawActive(false); });

		CreateBack();
		CreateMain();
		CreateStage();
		CreateNum();

		SetAlphaActive(true);
		m_selectBack->SetDrawActive(false);

		auto sinScaling = AddComponent<SinScaling>();
		sinScaling->SetUpdateActive(false);
	}

	void StageSelectSpriteObject::OnUpdate() {
		m_selectBack->GetComponent<Transform>()->SetScale(transform->GetScale());
	}

	void StageSelectSpriteObject::CreateBack() {
		wstring names[] = {
			L"StageSelectMenuBack2_TX",
			L"StageSelectMenuBack_TX",
		};

		for (const auto& name : names) {
			const float Size = 0.5f;
			const Vec3 offset(0.0f, 0.0f, 0.0f);
			auto param = Builder::VertexPCTParametor(Vec3(512.0f, 512.0f, 0.0f) * Size, Vec2(512.0f), name);
			auto object = GetStage()->AddGameObject<SpriteObject>(param);
			object->GetComponent<Transform>()->SetPosition(offset);
			object->SetParent(GetThis<GameObject>());

			(name == L"StageSelectMenuBack_TX") ? m_selectBack = object : m_unSelectBack = object;

			ex_weak_ptr<GameObject> exWeak = object;
			m_objects.push_back(exWeak);
		}
	}

	void StageSelectSpriteObject::CreateMain() {
		const float Size = 0.1125f;
		const Vec3 offset(-1.5f, 30.0f, 0.0f);
		auto param = Builder::VertexPCTParametor(Vec3(1280.0f, 1280.0f, 0.0f) * Size, Vec2(1280.0f, 720.0f), m_stageTexture);//L"SelectBack_TX");
		auto object = GetStage()->AddGameObject<SpriteObject>(param);
		object->GetComponent<Transform>()->SetPosition(offset);
		object->SetParent(GetThis<GameObject>());

		m_main = object;
		ex_weak_ptr<GameObject> exWeak = object;
		m_objects.push_back(exWeak);
	}

	void StageSelectSpriteObject::CreateStage() {
		const float Size = 1.0f;
		const Vec3 offset(-30.0f, -85.0f, 0.0f);
		auto param = Builder::VertexPCTParametor(Vec3(128.0f, 32.0f, 0.0f) * Size, Vec2(128.0f, 32.0f), L"Stage_TX");
		auto object = GetStage()->AddGameObject<SpriteObject>(param);
		object->GetComponent<Transform>()->SetPosition(offset);
		object->SetParent(GetThis<GameObject>());

		m_stage = object;
		ex_weak_ptr<GameObject> exWeak = object;
		m_objects.push_back(exWeak);
	}

	void StageSelectSpriteObject::CreateNum() {
		const float Size = 0.35f;
		const Vec3 offset(57.0f, -67.5f, 0.0f);
		auto number = GetStage()->AddGameObject<Number>(m_num);
		auto numberTrans = number->GetComponent<Transform>();
		auto scale = numberTrans->GetScale();
		numberTrans->SetScale(Vec3(1.5f, 1.0f, 0.0f) * Size);
		numberTrans->SetPosition(offset);
		number->SetParent(GetThis<GameObject>());

		m_number = number;
		ex_weak_ptr<GameObject> exWeak = number;
		m_objects.push_back(exWeak);
	}

}