/*!
@file   GoarObject.cpp
@brief  GoarObjectŽÀ‘Ì
*/

#include "stdafx.h"

#include "GoarObject.h"

#include "Watanabe/Utility/DataExtracter.h"
#include "Watanabe/Utility/AdvMeshUtil.h"

#include "Goar.h"

namespace basecross {

	GoalObject::GoalObject(const shared_ptr<Stage>& stage) :
		StageObjectBase(stage, L"Goar")
	{}

	GoalObject::GoalObject(const shared_ptr<Stage>& stage, const wstring& line) :
		StageObjectBase(stage, L"Goar")
	{
		vector<wstring> tokens = DataExtracter::DelimitData(line);
		DataExtracter::TransformDataExtraction(tokens, m_transformData);
	}

	void GoalObject::OnCreate() {
		SettingModel();

		AddComponent<Goal>(Goal::Parametor(Team::West));

		auto collision = AddComponent<CollisionObb>();
		collision->SetAfterCollision(AfterCollision::None);
	}

	void GoalObject::SettingModel() {
		auto draw = AddComponent<PNTStaticDraw>();

		draw->SetMeshResource(L"DEFAULT_SPHERE");
		draw->SetDiffuse(Col4(0.0f, 1.0f, 0.0f, 0.5f));

		SetAlphaActive(true);
	}
}