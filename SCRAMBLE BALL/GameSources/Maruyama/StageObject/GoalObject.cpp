/*!
@file   GoalObject.cpp
@brief  GoalObject実体
*/

#include "stdafx.h"

#include "GoalObject.h"

#include "Watanabe/Utility/DataExtracter.h"
#include "Watanabe/Utility/AdvMeshUtil.h"
#include "Watanabe/Shader/StaticModelDraw.h"

#include "Goal.h"
#include "Maruyama/Utility/Component/Targeted.h"
#include "Maruyama/Enemy/Component/SelfAstarNodeController.h"

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
		if (teamType == L"Blue") {
			m_team = team::TeamType::Blue;
		}
		else if (teamType == L"Red") {
			m_team = team::TeamType::Red;
		}
		else if (teamType == L"Neutral") {
			m_team = team::TeamType::Neutral;
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

		//ターゲット設定
		AddComponent<Targeted>(Targeted::Parametor(TargetedPriority::GOAL));
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

		auto draw = AddComponent<StaticModelDraw>();
		draw->SetMeshToTransformMatrix(spanMat);
		draw->SetMultiMeshResource(L"Goal");

		// アウトライン関係
		draw->SetOutlineActive(true);

		auto shadow = AddComponent<Shadowmap>();
		shadow->SetMultiMeshResource(L"Goal");

		if (m_team == team::TeamType::Blue) {
			//draw->SetTextureResource();
			//draw->SetTextureResource(L"RedGoal_TX");
			//draw->SetModelTextureEnabled(false);
		}
		else if (m_team == team::TeamType::Red) {
			draw->SetTextureResource(L"RedGoal_TX");
			draw->SetModelTextureEnabled(false);
		}
		else {
			draw->SetTextureResource(L"GoalTx");
			draw->SetModelTextureEnabled(false);
		}

		//SetAlphaActive(true);
	}
}