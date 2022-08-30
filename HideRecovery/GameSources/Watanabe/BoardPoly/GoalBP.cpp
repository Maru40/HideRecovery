#include "stdafx.h"
#include "GoalBP.h"
#include "BillBoard.h"
#include "../Manager/SpriteDataManager.h"
#include "../Utility/Utility.h"
#include "../DebugClass/Debug.h"

namespace basecross {
	GoalBP::GoalBP(const shared_ptr<Stage>& stage)
		:StageObjectBase(stage, L"GoalBP")
	{}

	shared_ptr<StageObjectBase> GoalBP::CreateBoard(const wstring& spriteDataKey) {
		auto spriteData = SpriteDataManager::GetInstance()->GetSpriteData(spriteDataKey);
		Vec2 origin = spriteData.origin;
		Vec2 size = spriteData.size;
		// テクスチャ座標でUVを定義
		vector<Vec2> uv01, uv = {
			origin,
			origin + Vec2(size.x,0.0f),
			origin + Vec2(0.0f, size.y),
			origin + size
		};
		Utility::ConvertToUVCoordinates(uv, spriteData.useTextureKey, uv01);

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

		auto gameObject = GetStage()->AddGameObject<StageObjectBase>(spriteDataKey);
		auto drawComp = gameObject->AddComponent<PTStaticDraw>();
		drawComp->SetOriginalMeshUse(true);
		drawComp->CreateOriginalMesh(vertices, indices);
		drawComp->SetTextureResource(spriteData.useTextureKey);
		drawComp->SetDiffuse(Col4(1.0f, 0.5f, 0, 1));

		auto billBoard = gameObject->AddComponent<BillBoard>();
		billBoard->SetRotationOffset(
			Utility::ConvertDegVecToRadVec(Vec3(0, 180, 0)));

		gameObject->SetAlphaActive(true);
		return gameObject;
	}

	void GoalBP::OnCreate() {
		auto goal = CreateBoard(L"Goal");
		auto goalTrans = goal->GetComponent<Transform>();
		goalTrans->SetPosition(Vec3(0, +1.0f / 2.0f, 0));
		goalTrans->SetScale(Vec3(2));
		goal->SetParent(GetThis<GoalBP>());

		auto triangle = CreateBoard(L"Triangle");
		auto triangleTrans = triangle->GetComponent<Transform>();
		triangleTrans->SetPosition(Vec3(0, -1.0f / 2.0f, 0));
		triangleTrans->SetScale(Vec3(0.5f));
		triangle->SetParent(GetThis<GoalBP>());
	}
	void GoalBP::OnUpdate() {
	}
}