
/*!
@file WallObject.cpp
@brief WallObjectクラス実体
担当：丸山裕喜
*/


#include "stdafx.h"
#include "Project.h"

#include "WallObject.h"

namespace basecross {
	
	void WallObject::SettingDraw() {
		auto drawComponent = AddComponent<PNTStaticDraw>();
		drawComponent->SetMeshResource(L"DEFAULT_CUBE");
		//drawComponent->SetDiffuse(Col4(0.1f, 0.1f, 0.8f, 1.0f));
		if (GetTexture() != L"") {
			drawComponent->SetTextureResource(GetTexture());
		}
	}

	void WallObject::OnCreate() {
		StageObjectBase::OnCreate();

		SettingDraw();

		auto col = AddComponent<CollisionObb>();
		col->SetFixed(true);
		AddTag(L"T_Obstacle");
		AddTag(L"Wall");
	}

}

//endbasecross