/*!
@file   GoalObject.cpp
@brief  GoalObject実体
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
		size_t nextIndex = DataExtracter::TransformDataExtraction(tokens, m_transformData);

		wstring teamType = tokens[nextIndex];
		if (teamType == L"East") {
			m_team = team::TeamType::East;
		}
		else if (teamType == L"West") {
			m_team = team::TeamType::West;
		}
		else {
			throw BaseException(
				L"BlockTypeが不正な値です。",
				L"BlockType : " + teamType,
				L"Block::Block(const shared_ptr<Stage>& stage, const wstring& line)"
			);
		}
	}

	void GoalObject::OnCreate() {
		SettingModel();

		AddComponent<SoundEmitter>();
		AddComponent<Goal>(Goal::Parametor(m_team));

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

		auto draw = AddComponent<PNTStaticModelDraw>();
		draw->SetMeshToTransformMatrix(spanMat);
		draw->SetMultiMeshResource(L"Goal");

		//SetAlphaActive(true);
	}
}