#include "stdafx.h"
#include "Container.h"
#include "../Utility/DataExtracter.h"

namespace basecross {
	Container::Container(const shared_ptr<Stage>& stage)
		:StageObjectBase(stage, L"Container")
	{}
	Container::Container(const shared_ptr<Stage>& stage, const wstring& line)
		: StageObjectBase(stage, L"Container")
	{
		vector<wstring> tokens = DataExtracter::DelimitData(line);
		DataExtracter::TransformDataExtraction(tokens, m_transformData);
	}

	void Container::OnCreate() {
		auto drawComp = AddComponent<PNTStaticModelDraw>();
		drawComp->SetMultiMeshResource(L"Container");
		drawComp->SetOwnShadowActive(true);

		auto shadow = AddComponent<Shadowmap>();
		shadow->SetMultiMeshResource(L"Container");

		AddTag(L"T_Obstacle");
		AddTag(L"Wall");
	}
}