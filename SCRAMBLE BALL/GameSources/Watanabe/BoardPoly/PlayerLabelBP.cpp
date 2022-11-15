#include "stdafx.h"
#include "PlayerLabelBP.h"
#include "../Utility/Utility.h"
#include "Maruyama/Utility/Component/BillBoard.h"
#include "Maruyama/Interface/I_TeamMember.h"

namespace basecross {
	PlayerLabelBP::PlayerLabelBP(const shared_ptr<Stage>& stage,
		const wstring& teamStr, int number)
		:StageObjectBase(stage, L"PlayerLabelBP"), m_teamStr(teamStr), m_number(number)
	{}

	shared_ptr<StageObjectBase> PlayerLabelBP::CreateBoard(const sdm::SpriteData& spriteData) {
		Vec2 origin = spriteData.origin;
		Vec2 size = spriteData.size;
		// テクスチャ座標でUVを定義
		vector<Vec2> uv01, uv = {
			origin,
			origin + Vec2(size.x,0.0f),
			origin + Vec2(0.0f, size.y),
			origin + size
		};
		TextureUtility::ConvertToUVCoordinates(uv, spriteData.useTextureKey, uv01);

		// サイズを1以内に納める
		Vec2 size01 = Utility::ConvertVec2to01(size);
		Vec2 halfSize = size01 / 2.0f;

		// 頂点のデータ (番号は左上から右下まで)
		vector<VertexPositionTexture> vertices = {
			{Vec3(-halfSize.x,+halfSize.y,0.0f),uv01[0]}, //0
			{Vec3(+halfSize.x,+halfSize.y,0.0f),uv01[1]}, //1

			{Vec3(-halfSize.x,-halfSize.y,0.0f),uv01[2]}, //2
			{Vec3(+halfSize.x,-halfSize.y,0.0f),uv01[3]},  //3
		};

		// 頂点インデックス
		vector<uint16_t> indices = {
			0, 1, 2, // 上の三角形
			2, 1, 3  // 下の三角形
		};

		auto gameObject = GetStage()->AddGameObject<StageObjectBase>(spriteData.useTextureKey);
		auto drawComp = gameObject->AddComponent<PTStaticDraw>();
		drawComp->SetOriginalMeshUse(true);
		drawComp->CreateOriginalMesh(vertices, indices);
		drawComp->SetTextureResource(spriteData.useTextureKey);

		gameObject->SetAlphaActive(true);
		return gameObject;
	}

	void PlayerLabelBP::OnCreate() {
		auto spriteData = SpriteDataManager::GetInstance()->GetSpriteData(m_teamStr);
		auto team = CreateBoard(spriteData);
		auto teamTrans = team->GetComponent<Transform>();
		teamTrans->SetPosition(Vec3(-0.1f, 0, 0));
		teamTrans->SetScale(Vec3(0.7f));
		team->SetParent(GetThis<PlayerLabelBP>());

		spriteData = SpriteDataManager::GetInstance()->GetSpriteData(L"Numbers");
		spriteData.origin.x = spriteData.size.x * m_number;
		auto number = CreateBoard(spriteData);
		auto numberTrans = number->GetComponent<Transform>();
		numberTrans->SetPosition(Vec3(+0.4f, 0, 0));
		numberTrans->SetScale(Vec3(0.273f));
		number->SetParent(GetThis<PlayerLabelBP>());

		auto billBoard = AddComponent<BillBoard>();
		billBoard->SetRotationOffset(
			Utility::ConvertDegVecToRadVec(Vec3(0, 180, 0)));

		shared_ptr<GameObject> objs[] = {
			team,number
		};
		for (auto obj : objs) {
			auto drawComp = obj->GetComponent<PTStaticDraw>();
			if (m_teamStr == L"Red") {
				drawComp->SetDiffuse(team::REDTEAM_COLOR);
			}
			else if (m_teamStr == L"Blue") {
				drawComp->SetDiffuse(team::BLUETEAM_COLOR);
			}
		}

		GetComponent<Transform>()->SetScale(Vec3(0.7f));

		SetActive(false);
	}

	void PlayerLabelBP::OnUpdate() {
	}
}