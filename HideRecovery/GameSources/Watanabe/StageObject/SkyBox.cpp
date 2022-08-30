#include "stdafx.h"
#include "SkyBox.h"
#include "Itabashi/PCTStaticModelDraw.h"

namespace basecross {
	SkyBox::SkyBox(const shared_ptr<Stage>& stage, const Vec3& scale)
		:StageObjectBase(stage, L"SkyBox")
	{
		m_transformData.Scale = scale;
	}

	void SkyBox::OnCreate() {
		auto drawComp = AddComponent<PCTStaticModelDraw>();
		drawComp->SetMeshResource(L"SkyBox_Model");
		drawComp->SetSamplerState(SamplerState::LinearClamp); // テクスチャの切れ目を目立たなくする（これをやらないとプレーンの境目に線が入ったように見えてしまう）
	}
}