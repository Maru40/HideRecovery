#include "stdafx.h"
#include "Container.h"
#include "../Utility/DataExtracter.h"

namespace basecross {
	Container::Container(const shared_ptr<Stage>& stage)
		:StageObjectBase(stage, L"Container"), m_color(L"Green")
	{}
	Container::Container(const shared_ptr<Stage>& stage, const wstring& line)
		: StageObjectBase(stage, L"Container")
	{
		vector<wstring> tokens = DataExtracter::DelimitData(line);
		size_t nextIndex = DataExtracter::TransformDataExtraction(tokens, m_transformData);

		m_color = tokens[nextIndex];
	}

	void Container::OnCreate() {
		auto drawComp = AddComponent<PNTStaticModelDraw>();
		drawComp->SetMultiMeshResource(L"Container");
		drawComp->SetOwnShadowActive(true);
		// モデルのテクスチャを無効にして、別のテクスチャを使用
		drawComp->SetModelTextureEnabled(false);
		drawComp->SetTextureResource(m_color + L"ContianerTx");

		auto shadow = AddComponent<Shadowmap>();
		shadow->SetMultiMeshResource(L"Container");

		m_hitTestObject = GetStage()->AddGameObject<GameObject>();
		auto transComp = m_hitTestObject->GetComponent<Transform>();
		transComp->SetPosition(transform->GetPosition() + Vec3(0, 2, 0));
		transComp->SetQuaternion(transform->GetQuaternion());
		transComp->SetScale(Vec3(4, 4, 8.5));

		auto collision = m_hitTestObject->AddComponent<CollisionObb>();
		collision->SetFixed(true);
		m_hitTestObject->AddTag(L"T_Obstacle");
		m_hitTestObject->AddTag(L"Wall");
	}
}