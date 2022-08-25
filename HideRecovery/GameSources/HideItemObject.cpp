/*!
@file HideItemObject.cpp
@brief HideItemObjectクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "HideItemObject.h"

#include "HideItem.h"
#include "Targeted.h"
#include "Itabashi/ObjectHider.h"
#include "Itabashi/Item.h"
#include "Watanabe/Utility/DataExtracter.h"
#include "Watanabe/Component/BallAnimator.h"

#include "PlayerInputer.h"

namespace basecross {
	HideItemObject::HideItemObject(const std::shared_ptr<Stage>& stage) :
		StageObjectBase(stage, L"HideItem")
	{}

	HideItemObject::HideItemObject(const std::shared_ptr<Stage>& stage, const std::wstring& line) :
		StageObjectBase(stage, L"HideItem")
	{
		vector<wstring> tokens = DataExtracter::DelimitData(line);
		size_t nextIndex = DataExtracter::TransformDataExtraction(tokens, m_transformData);
	}

	void HideItemObject::OnCreate() {
		auto ti = GetThis<GameObject>();
		SettingModel();

		AddComponent<Operator::ObjectHider>();
		auto item = AddComponent<Item>();

		// 仮のアイテムID
		item->SetItemId(100);

		AddComponent<HideItem>();
		AddComponent<Targeted>();

		auto animator = AddComponent<BallAnimator>();
		animator->ChangeBallAnimation(BallAnimationState::State::Gwait);
	}

	void HideItemObject::OnUpdate() {
		//デバッグ-----------------------------------------------------------------

		auto animator = GetComponent<BallAnimator>(false);

		if (PlayerInputer::GetInstance()->IsLeftDown()) {
			animator->ChangeBallAnimation(BallAnimationState::State::Goal);
		}

		//-------------------------------------------------------------------------
	}

	void HideItemObject::SettingModel() {
		Mat4x4 spanMat;
		const float fScale = 1.0f;
		Vec3 scale = Vec3(fScale);
		spanMat.affineTransformation(
			scale,
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		auto draw = AddComponent<DrawComp>();
		draw->SetMultiMeshResource(L"Ball_Model");
		draw->SetDiffuse(Col4(0.0f, 0.5f, 0.0f, 1.0f));
		draw->SetMeshToTransformMatrix(spanMat);

		transform->SetScale(Vec3(0.5f));
	}
}