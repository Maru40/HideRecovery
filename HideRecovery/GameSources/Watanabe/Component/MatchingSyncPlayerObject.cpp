#include "stdafx.h"
#include "MatchingSyncPlayerObject.h"
#include "PlayerAnimator.h"
#include "VelocityManager.h"
#include "../DebugClass/Debug.h"
#include "../Effekseer/EfkEffect.h"

namespace basecross {
	MatchingSyncPlayerObject::MatchingSyncPlayerObject(const shared_ptr<GameObject>& owner,
		const vector<shared_ptr<PlayerSpawnPointObject>>& pointObjects,
		shared_ptr<Online::OnlineMatching> onlineMatching)
		:Online::OnlineComponent(owner), m_onlineMatching(onlineMatching)
	{
		if (pointObjects.size() != Online::OnlineMatching::MAX_PLAYER_NUM) {
			throw BaseException(
				L"スポーンポイントの数と最大マッチング人数が一致しません",
				L"pointObjects : " + Util::IntToWStr((int)pointObjects.size()) + L"\nMAX_PLAYER_NUM : " + Util::IntToWStr(Online::OnlineMatching::MAX_PLAYER_NUM),
				L"MatchingSyncPlayerObject::OnCreate()"
			);
		}

		for (auto pointObject : pointObjects) {
			auto player = CreatePlayerModel();
			auto playerTrans = player->GetComponent<Transform>();
			auto spawnTrans = pointObject->GetComponent<Transform>();
			playerTrans->SetPosition(spawnTrans->GetPosition());
			player->SetActive(false);
			m_players.push_back(player);
		}
	}

	void MatchingSyncPlayerObject::OnCreate() {
	}

	shared_ptr<StageObjectBase> MatchingSyncPlayerObject::CreatePlayerModel() {
		Mat4x4 spanMat;
		const float fScale = 0.8f;
		Vec3 scale = Vec3(fScale);
		spanMat.affineTransformation(
			scale,
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);
		auto gameObject = GetStage()->AddGameObject<StageObjectBase>(L"PlayerModel");
		auto drawComp = gameObject->AddComponent<PNTBoneModelDraw>();
		drawComp->SetMultiMeshResource(L"Player_Mesh");
		drawComp->SetMeshToTransformMatrix(spanMat);

		auto animator = gameObject->AddComponent<PlayerAnimator>();
		animator->ChangePlayerAnimation(PlayerAnimationState::State::Wait);

		gameObject->AddComponent<VelocityManager>();

		// エフェクトの設定
		auto efkComp = gameObject->AddComponent<EfkComponent>();
		efkComp->SetEffectResource(L"Respawn", TransformData(Vec3(0, -0.5f, 0), Vec3(0.3f, 2.0f, 0.3f)));

		return gameObject;
	}

	void MatchingSyncPlayerObject::OnJoinRoomEventAction(int playerNumber, const std::vector<int>& playerNumbers, const ExitGames::LoadBalancing::Player& player) {
		for (int i = 0; i < playerNumbers.size(); i++) {
			auto efkComp = m_players[i]->GetComponent<EfkComponent>(false);
			if (efkComp && !m_players[i]->IsActive()) {
				efkComp->Play(L"Respawn");
			}
			m_players[i]->SetActive(true);
		}
	}
	void MatchingSyncPlayerObject::OnLeaveRoomEventAction(int playerNumber, bool isInactive) {
		for (int i = 0; i < m_players.size(); i++) {
			if (!m_players[i]->IsActive()) {
				m_players[(int)i - 1]->SetActive(false);
			}
		}
	}
}