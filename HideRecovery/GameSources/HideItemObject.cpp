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

#include "Watanabe/Utility/DataExtracter.h"

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
		SettingModel();

		AddComponent<HideItem>();
		AddComponent<Targeted>();
	}

	void HideItemObject::SettingModel() {
		auto draw = AddComponent<DrawComp>();
		draw->SetMeshResource(L"DEFAULT_SPHERE");
		draw->SetDiffuse(Col4(0.0f, 0.5f, 0.0f, 1.0f));

		transform->SetScale(Vec3(0.5f));
	}
}