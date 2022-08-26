﻿/*!
@file   RackObject.cpp
@brief  棚オブジェクトクラス実体
*/

#include "stdafx.h"
#include "RackObject.h"
#include "HidePlace.h"
#include "../Utility/DataExtracter.h"

#include "Watanabe/Component/BoxAnimator.h"

namespace basecross {
	RackObject::RackObject(const shared_ptr<Stage>& stage)
		:StageObjectBase(stage, L"RackObject")
	{}

	RackObject::RackObject(const shared_ptr<Stage>& stage, const wstring& line)
		: StageObjectBase(stage, L"RackObject")
	{
		vector<wstring> tokens = DataExtracter::DelimitData(line);
		DataExtracter::TransformDataExtraction(tokens, m_transformData);
	}

	void RackObject::OnCreate() {
		auto col = AddComponent<CollisionObb>();
		col->SetFixed(true);
		const float colSize = 1.2f;
		col->SetMakedSize(colSize);

		auto drawComp = AddComponent<PNTBoneModelDraw>();
		//drawComp->SetMeshResource(L"rack");
		drawComp->SetMultiMeshResource(L"Box_Model");
		drawComp->SetOwnShadowActive(true);

		auto animator = AddComponent<BoxAnimator>();
		 
		// 隠せるオブジェクトにする
		AddComponent<HidePlace>(HidePlace::Parametor(Vec3(0.0f, 0.15f, 0.0f)));

		//constexpr float fScale = 0.75f;
		//transform->SetScale(Vec3(fScale));

		auto shadow = AddComponent<Shadowmap>();
		//shadow->SetMeshResource(L"Box_Model");
	}

	void RackObject::OnUpdate() {

	}
}