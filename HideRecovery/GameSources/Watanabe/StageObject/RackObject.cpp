/*!
@file   RackObject.cpp
@brief  棚オブジェクトクラス実体
*/

#include "stdafx.h"
#include "RackObject.h"
#include "HidePlace.h"

namespace basecross {
	RackObject::RackObject(const shared_ptr<Stage>& stage)
		:GameObject(stage)
	{}

	RackObject::RackObject(const shared_ptr<Stage>& stage, const wstring& line)
		: GameObject(stage)
	{}

	void RackObject::OnCreate() {
		auto drawComp = AddComponent<PNTStaticModelDraw>();
		drawComp->SetMeshResource(L"rack");

		// 隠せるオブジェクトにする
		AddComponent<HidePlace>(HidePlace::Parametor(Vec3(0.0f, 1.85f, 0.0f)));
	}
}