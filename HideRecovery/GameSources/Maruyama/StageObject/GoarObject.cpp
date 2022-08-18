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

	GoarObject::GoarObject(const shared_ptr<Stage>& stage) :
		StageObjectBase(stage, L"Goar")
	{}

	GoarObject::GoarObject(const shared_ptr<Stage>& stage, const wstring& line) :
		StageObjectBase(stage, L"Goar")
	{
		vector<wstring> tokens = DataExtracter::DelimitData(line);
		DataExtracter::TransformDataExtraction(tokens, m_transformData);
	}

	void GoarObject::OnCreate() {
		SettingModel();

		AddComponent<Goar>();
	}

	void GoarObject::SettingModel() {
		auto draw = AddComponent<PNTStaticDraw>();

		draw->SetMeshResource(L"DEFAULT_SPHERE");
		draw->SetDiffuse(Col4(0.0f, 1.0f, 0.0f, 0.5f));

		SetAlphaActive(true);
	}
}