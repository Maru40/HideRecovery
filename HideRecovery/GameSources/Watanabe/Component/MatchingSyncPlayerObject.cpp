#include "stdafx.h"
#include "MatchingSyncPlayerObject.h"
#include "PlayerAnimator.h"
#include "VelocityManager.h"

namespace basecross {
	MatchingSyncPlayerObject::MatchingSyncPlayerObject(const shared_ptr<GameObject>& owner,
		const vector<shared_ptr<PlayerSpawnPointObject>>& pointObjects,
		shared_ptr<Online::OnlineMatching> onlineMatching)
		:Online::OnlineComponent(owner), m_spawnPoints(pointObjects), m_onlineMatching(onlineMatching)
	{}

	void MatchingSyncPlayerObject::OnCreate() {
		if (m_spawnPoints.size() != Online::OnlineMatching::MAX_PLAYER_NUM) {
			throw BaseException(
				L"スポーンポイントの数と最大マッチング人数が一致しません",
				L"SpawnPointCount : " + Util::IntToWStr((int)m_spawnPoints.size()) + L"\nMAX_PLAYER_NUM : " + Util::IntToWStr(Online::OnlineMatching::MAX_PLAYER_NUM),
				L"MatchingSyncPlayerObject::OnCreate()"
			);
		}
	}

	shared_ptr<GameObject> MatchingSyncPlayerObject::CreatePlayerModel() {
		Mat4x4 spanMat;
		const float fScale = 0.8f;
		Vec3 scale = Vec3(fScale);
		spanMat.affineTransformation(
			scale,
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);
		auto gameObject = GetStage()->AddGameObject<GameObject>();
		auto drawComp = gameObject->AddComponent<PNTBoneModelDraw>();
		drawComp->SetMultiMeshResource(L"Player_Mesh");
		drawComp->SetMeshToTransformMatrix(spanMat);

		auto animator = gameObject->AddComponent<PlayerAnimator>();
		animator->ChangePlayerAnimation(PlayerAnimationState::State::Wait);

		gameObject->AddComponent<VelocityManager>();

		return gameObject;
	}

	void MatchingSyncPlayerObject::OnCreateRoom() {
		for (auto spawnPoint : m_spawnPoints) {
			if (spawnPoint->GetID() == 0) {
				auto player = CreatePlayerModel();
				auto playerTrans = player->GetComponent<Transform>();
				auto spawnTrans = spawnPoint->GetComponent<Transform>();
				playerTrans->SetPosition(spawnTrans->GetPosition());
			}
		}
	}
	void MatchingSyncPlayerObject::OnJoinRoom() {
		auto playerCount = m_onlineMatching->GetPlayerCount();
		for (int i = 0; i < playerCount; i++) {
			auto spawnPoint = m_spawnPoints[i];
			auto player = CreatePlayerModel();
			auto playerTrans = player->GetComponent<Transform>();
			auto spawnTrans = spawnPoint->GetComponent<Transform>();
			playerTrans->SetPosition(spawnTrans->GetPosition());
		}
	}
}