/*!
@file   GoalObject.cpp
@brief  GoalObject����
*/

#include "stdafx.h"

#include "GoalObject.h"

#include "Watanabe/Utility/DataExtracter.h"
#include "Watanabe/Utility/AdvMeshUtil.h"
#include "Watanabe/StageObject/PlayerSpawnPointObject.h"

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
			m_team = Team::East;
		}
		else if (teamType == L"West") {
			m_team = Team::West;
		}
		else {
			throw BaseException(
				L"BlockType���s���Ȓl�ł��B",
				L"BlockType : " + teamType,
				L"Block::Block(const shared_ptr<Stage>& stage, const wstring& line)"
			);
		}
	}

	void GoalObject::OnCreate() {
		SettingModel();

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

		auto draw = AddComponent<PNTStaticDraw>();
		draw->SetMeshToTransformMatrix(spanMat);
		draw->SetMultiMeshResource(L"Goal");

		//if (m_team == Team::East) {
		//	draw->SetDiffuse(Col4(1.0f, 0.0f, 0.0f, 1.0f));
		//}
		//else {
		//	draw->SetDiffuse(Col4(0.0f, 0.0f, 1.0f, 1.0f));
		//}

		//SetAlphaActive(true);
	}
}