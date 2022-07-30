/*!
@file ParticleManager.cpp
@brief ParticleManagerのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "ClearUIObject.h"

#include "SpriteObject.h"

namespace basecross {

	ClearUIObject::ClearUIObject(const std::shared_ptr<Stage>& stage):
		GameObject(stage)
	{}

	void ClearUIObject::OnCreate() {
		CreateBackUI();
		CreateClearUI();
	}

	void ClearUIObject::CreateBackUI() {
		auto color = Col4(0.2f, 0.2f, 0.2f, 0.5f);
		auto parametor = Builder::VertexPCTParametor(Vec3(2048.0f, 256.0f, 0.0f), color, Vec2(256.0f, 256.0f), L"SquareSpriteAlpha_TX");
		auto offset = Vec3(0.0f, 0.0f, 0.0f);

		auto ui = CreateUI(parametor, offset);
		m_back = ui;
	}

	void ClearUIObject::CreateClearUI() {
		auto parametor = Builder::VertexPCTParametor(Vec3(1024.0f, 256.0f, 0.0f) , Vec2(1024.0f, 256.0f), L"Clear_TX");
		auto offset = Vec3(0.0f, 0.0f, 0.0f);

		auto ui = CreateUI(parametor, offset);
		m_clear = ui;
	}

	std::shared_ptr<SpriteObject> ClearUIObject::CreateUI(const Builder::VertexPCTParametor& vertexParam, const Vec3& position) {
		auto ui = GetStage()->AddGameObject<SpriteObject>(vertexParam);

		ui->GetComponent<Transform>()->SetPosition(position);
		ui->SetParent(GetThis<GameObject>());

		m_allObjects.push_back(ui);
		return ui;
	}

	std::shared_ptr<SpriteObject> ClearUIObject::GetBackUI() const {
		return m_back.GetShard();
	}

	std::shared_ptr<SpriteObject> ClearUIObject::GetClearUI() const {
		return m_clear.GetShard();
	}

}