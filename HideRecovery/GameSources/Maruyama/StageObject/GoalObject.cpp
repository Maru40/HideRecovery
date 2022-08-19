/*!
@file   GoalObject.cpp
@brief  GoalObjectŽÀ‘Ì
*/

#include "stdafx.h"

#include "GoalObject.h"

#include "Watanabe/Utility/DataExtracter.h"
#include "Watanabe/Utility/AdvMeshUtil.h"

#include "Goal.h"

namespace basecross {

	GoalObject::GoalObject(const shared_ptr<Stage>& stage) :
		StageObjectBase(stage, L"Goal")
	{}

	GoalObject::GoalObject(const shared_ptr<Stage>& stage, const wstring& line) :
		StageObjectBase(stage, L"Goal")
	{
		vector<wstring> tokens = DataExtracter::DelimitData(line);
		DataExtracter::TransformDataExtraction(tokens, m_transformData);
	}

	void GoalObject::OnCreate() {
		SettingModel();

		AddComponent<Goal>(Goal::Parametor(Team::West));

		auto collision = AddComponent<CollisionObb>();
		collision->SetAfterCollision(AfterCollision::None);
		//collision->SetDrawActive(true);
	}

	void GoalObject::SettingModel() {
		Mat4x4 spanMat;
		const float fScale = 1.0f;
		Vec3 scale = Vec3(fScale);
		spanMat.affineTransformation(
			scale,
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, -0.4f, 0.0f)
		);

		auto draw = AddComponent<PNTStaticDraw>();
		draw->SetMeshToTransformMatrix(spanMat);
		draw->SetMultiMeshResource(L"Goal");

		//SetAlphaActive(true);
	}
}