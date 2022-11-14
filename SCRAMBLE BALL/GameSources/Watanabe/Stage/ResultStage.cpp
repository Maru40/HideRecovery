#include "stdafx.h"
#include "ResultStage.h"
#include "Patch/PlayerInputer.h"
#include "Scene.h"

#include "Watanabe/DebugClass/Debug.h"
#include "Watanabe/UI/UIObjects.h"
#include "Watanabe/Effekseer/EfkEffect.h"

#include "../Manager/PointManager.h"
#include "../Manager/TimeManager.h"
#include "../Manager/ScoreManager.h"
#include "Itabashi/OnlineManager.h"
#include "Watanabe/Component/PlayerAnimator.h"
#include "Watanabe/Component/MatchingSyncPlayerObject.h"
#include "Watanabe/Component/PlayerStatus.h"

#include "Maruyama/Interface/I_TeamMember.h"
#include "VelocityManager.h"

namespace basecross {
	std::shared_ptr<GameObject> PlayerCreate(const std::shared_ptr<Stage>& stage) {
		Mat4x4 spanMat;
		const float fScale = 0.8f;
		Vec3 scale = Vec3(fScale);
		spanMat.affineTransformation(
			scale,
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, -0.55f, 0.0f)
		);
		auto gameObject = stage->AddGameObject<StageObjectBase>(L"PlayerModel");
		auto drawComp = gameObject->AddComponent<BoneModelDraw>();
		drawComp->SetMultiMeshResource(L"Player_Mesh");
		drawComp->SetMeshToTransformMatrix(spanMat);

		// アウトライン関係
		drawComp->SetOutlineActive(true);

		auto shadow = gameObject->AddComponent<Shadowmap>();
		shadow->SetMultiMeshResource(L"Player_Mesh");
		shadow->SetMeshToTransformMatrix(spanMat);

		auto animator = gameObject->AddComponent<PlayerAnimator>();
		animator->ChangePlayerAnimation(PlayerAnimationState::State::Wait);
		// PlayerAnimatorで参照しているため使用しないが必要
		gameObject->AddComponent<VelocityManager>();

		gameObject->AddComponent<PlayerStatus>();

		return gameObject;
	}

	void ResultStage::CreateViewLight() {
		const Vec3 eye(0.0f, 1.0f, 5.0f);
		const Vec3 at(0, 1.0f, 0);
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}

	void ResultStage::OnCreate() {
		CreateViewLight();
		AddGameObject<EfkInterface>();
		AddGameObject<Debug>();
		Debug::GetInstance()->Log(L"ResultStage");
		Debug::GetInstance()->Log(L"A : マッチング画面へ");
		Debug::GetInstance()->Log(L"B : タイトル画面へ");

		// デバッグ
		PointManager::GetInstance()->AddPoint(team::TeamType::Blue);

		auto timeLine = AddGameObject<GameObject>()->AddComponent<TimeLine>();
		timeLine->AddKeyFrame(CameraKeyFrameData(Vec3(-1, 1, 2), Vec3(-2, 0.5f, 0), 0, Lerp::rate::Cube));
		timeLine->AddKeyFrame(CameraKeyFrameData(Vec3(0, 1, 2), Vec3(-1, 0.5f, 0), 0.5f, Lerp::rate::Cube));
		timeLine->AddKeyFrame(CameraKeyFrameData(Vec3(1, 1, 2), Vec3(0, 0.5f, 0), 1.5f, Lerp::rate::Cube));
		timeLine->AddKeyFrame(CameraKeyFrameData(Vec3(2, 1, 2), Vec3(1, 0.5f, 0), 2.5f, Lerp::rate::Cube));
		timeLine->AddKeyFrame(CameraKeyFrameData(Vec3(0, 1, 5), Vec3(0, 1, 0), 3.5f, Lerp::rate::Cube));
		m_timeLine = timeLine;

		CreateMap(L"WaitStage.csv");
		auto uiBuilder = CreateUI(L"ResultUILayout.csv");
		// ラベルの色変更
		auto redLabel = uiBuilder->GetUIObject<SimpleSprite>(L"RedLabel");
		redLabel->GetDrawComponent()->SetDiffuse(team::REDTEAM_COLOR);
		auto blueLabel = uiBuilder->GetUIObject<SimpleSprite>(L"BlueLabel");
		blueLabel->GetDrawComponent()->SetDiffuse(team::BLUETEAM_COLOR);

		// 引き分け以外に生成
		if (!PointManager::GetInstance()->IsDraw()) {
			// 紙吹雪エフェクト
			auto effectObject = AddGameObject<GameObject>();
			effectObject->GetComponent<Transform>()->SetPosition(Vec3(0, 5, 0));
			auto efkComp = effectObject->AddComponent<EfkComponent>();
			efkComp->SetEffectResource(L"Confetti");
			efkComp->PlayLoop(L"Confetti");
		}

		//playerの生成
		CreatePlayers(PointManager::GetInstance()->GetWinner());

		Online::OnlineManager::Disconnect();

		timeLine->Play();
	}

	void ResultStage::OnUpdate() {
		// 各画面への遷移処理
		// Aでマッチング画面、Bでタイトル画面
		const auto& inputDevice = App::GetApp()->GetMyInputDevice();
		const auto& pad = inputDevice->GetXInputGamePad();
		if (pad.IsInputDown(XInputCode::A)) {
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToMatchingStage");
		}
		else if (pad.IsInputDown(XInputCode::B)) {
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
		}

		else if (pad.IsInputDown(XInputCode::Y)) {
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToResultStage");
		}
	}

	void ResultStage::OnDestroy() {
		// リザルトを抜ける際にインスタンスを破棄（リセット）
		PointManager::DeleteInstance();
		TimeManager::DeleteInstance();
		ScoreManager::DeleteInstance();
	}

	void ResultStage::CreatePlayers(const team::TeamType winerType) {
		if (PointManager::GetInstance()->IsDraw()) {
			return;
		}

		struct Data {
			Vec3 position;
			PlayerAnimationState::State state;
			float time;

			Data(const Vec3& position, const PlayerAnimationState::State& state, float time) :
				position(position),
				state(state),
				time(time)
			{}
		};

		Data datas[] = {
			Data(Vec3(-1.0f, 0.1f, 0.0f), PlayerAnimationState::State::Win2,0.5f),
			Data(Vec3(+0.0f, 0.1f, 0.0f), PlayerAnimationState::State::Win1,1.5f),
			Data(Vec3(+1.0f, 0.1f, 0.0f), PlayerAnimationState::State::Win3,2.5f),
		};

		for (auto& data : datas) {
			auto player = PlayerCreate(GetThis<Stage>());
			player->GetComponent<Transform>()->SetPosition(data.position);
			auto teamMember = player->GetComponent<I_TeamMember>(false);
			teamMember->SetTeam(winerType);

			auto animator = player->GetComponent<PlayerAnimator>(false);
			animator->ChangePlayerAnimation(PlayerAnimationState::State::Wait);

			if (auto timeLine = m_timeLine.lock()) {
				timeLine->AddEvent(data.time,
					[&, animator, data]() {
						animator->ChangePlayerAnimation(data.state);
					}
				);
			}
		}
	}
}