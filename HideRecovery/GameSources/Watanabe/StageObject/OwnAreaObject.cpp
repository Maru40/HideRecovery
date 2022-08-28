/*!
@file   OwnAreaObject.cpp
@brief  自陣エリアクラス実体
*/

#include "stdafx.h"
#include "OwnAreaObject.h"
#include "../Utility/DataExtracter.h"
#include "../DebugClass/Debug.h"
#include "../Utility/AdvMeshUtil.h"

#include "Maruyama/Player/Component/OwnArea.h"

namespace basecross {
	OwnAreaObject::OwnAreaObject(const shared_ptr<Stage>& stage)
		:StageObjectBase(stage, L"OwnArea"), m_areaRadius(0), m_team(team::TeamType::Blue)
	{}
	OwnAreaObject::OwnAreaObject(const shared_ptr<Stage>& stage, const wstring& line)
		: StageObjectBase(stage, L"OwnArea"), m_areaRadius(0)
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
		else {
			throw BaseException(
				L"Teamが不正な値です。",
				L"team::TeamType : " + teamType,
				L"OwnAreaObject::OwnAreaObject()"
			);
		}
	}

	void OwnAreaObject::OnCreate() {
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetSamplerState(SamplerState::LinearWrap);
		drawComp->SetOwnShadowActive(true);

		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;

		// スケールに応じたUVを持つCubeを設定
		AdvMeshUtil::CreateCube(4.0f, m_transformData.Scale, vertices, indices);
		m_meshRes = MeshResource::CreateMeshResource(vertices, indices, true);
		drawComp->SetMeshResource(m_meshRes);
		drawComp->SetTextureResource(L"Floor_TX");
		AddTag(L"Floor");

		// 仮で色を変える
		{
			switch (m_team)
			{
			case team::TeamType::Blue:
				drawComp->SetDiffuse(Col4(0, 0, 1, 1));
				break;
			case team::TeamType::Red:
				drawComp->SetDiffuse(Col4(1, 0, 0, 1));
				break;
			default:
				break;
			}
		}

		auto col = AddComponent<CollisionObb>();
		col->SetFixed(true);
		col->SetAfterCollision(AfterCollision::None);

		AddComponent<OwnArea>(OwnArea::Parametor(m_team, m_areaRadius));
	}
	void OwnAreaObject::OnUpdate() {
	}
}