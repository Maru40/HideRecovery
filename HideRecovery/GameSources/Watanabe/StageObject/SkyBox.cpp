#include "stdafx.h"
#include "SkyBox.h"

namespace basecross {
	SkyBox::SkyBox(const shared_ptr<Stage>& stage, const Vec3& scale)
		:StageObjectBase(stage, L"SkyBox")
	{
		m_transformData.Scale = scale;
	}

	void SkyBox::OnCreate() {
		auto drawComp = AddComponent<PNTStaticModelDraw>();
		drawComp->SetMeshResource(L"SkyBox_Model");
		drawComp->SetDiffuse(Col4(1));
		drawComp->SetEmissive(Col4(1));
		drawComp->SetSamplerState(SamplerState::LinearClamp); // テクスチャの切れ目を目立たなくする（これをやらないとプレーンの境目に線が入ったように見えてしまう）
	}
}