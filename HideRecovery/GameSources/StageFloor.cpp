#include "stdafx.h"
#include "StageFloor.h"

namespace basecross {

	StageFloor::StageFloor(const std::shared_ptr<Stage>& stage, const wstring& name, const wstring& texture)
		:StageObjectBase(stage, name, texture)
	{ }

	void StageFloor::OnCreate()
	{
		StageObjectBase::OnCreate();

		// 地面をキューブで置く
		auto drawComponent = AddComponent<PNTStaticDraw>();
		drawComponent->SetMeshResource(L"DEFAULT_CUBE");
		drawComponent->SetOwnShadowActive(true);
		drawComponent->SetTextureResource(L"Floor_TX");

		// 地面の位置情報に関する情報
		auto transComponent = GetComponent<Transform>();
		transComponent->SetPosition(0.0f, 0.0f, 0.0f); // 位置情報の設定
		transComponent->SetScale(20.0f, 1.0f, 20.0f); // スケールの変更

		// 物理演算の追加
		auto ptrColl = AddComponent<CollisionObb>();
		//ptrColl->SetAfterCollision(AfterCollision::None);
		ptrColl->SetFixed(true);

		AddTag(L"StageFloor"); // 地面にタグをつける

	}

	void StageFloor::OnUpdate()
	{
		// 特にない
	}
}

