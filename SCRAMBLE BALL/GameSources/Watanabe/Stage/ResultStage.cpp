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
#include "../TimeLine/IncTimeLine.h"

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

		CreateMap(L"WaitStage.csv");
		auto uiBuilder = CreateUI(L"ResultUILayout.csv");
		// ラベルの色変更
		auto redLabel = uiBuilder->GetUIObject<SimpleSprite>(L"RedLabel");
		redLabel->GetDrawComponent()->SetDiffuse(team::REDTEAM_COLOR);
		auto blueLabel = uiBuilder->GetUIObject<SimpleSprite>(L"BlueLabel");
		blueLabel->GetDrawComponent()->SetDiffuse(team::BLUETEAM_COLOR);

		AddGameObject<timeline::TimeLine>();

		const bool IsDraw = PointManager::GetInstance()->IsDraw();
		// 引き分け以外
		if (!IsDraw) {
			// カメラクリップの作成
			auto cameraClip = timeline::TimeLine::GetInstance()->CreateClip<timeline::CameraClip>(L"Camera");
			cameraClip->SetTargetCamera(GetView()->GetTargetCamera());
			cameraClip->AddKeyFrame(timeline::CameraKeyFrame::Create(Vec3(-2, 1, 2), Vec3(-3, 0.5f, 0), 0, Lerp::rate::Cube));
			cameraClip->AddKeyFrame(timeline::CameraKeyFrame::Create(Vec3(-0.5f, 1, 2), Vec3(-1.5f, 0.5f, 0), 0.5f, Lerp::rate::Cube));
			cameraClip->AddKeyFrame(timeline::CameraKeyFrame::Create(Vec3(1, 1, 2), Vec3(0, 0.5f, 0), 1.5f, Lerp::rate::Cube));
			cameraClip->AddKeyFrame(timeline::CameraKeyFrame::Create(Vec3(2.5f, 1, 2), Vec3(1.5f, 0.5f, 0), 2.5f, Lerp::rate::Cos));
			cameraClip->AddKeyFrame(timeline::CameraKeyFrame::Create(Vec3(0, 1, 6), Vec3(0, 1, 0), 3.3f, Lerp::rate::Cos));
			cameraClip->AddKeyFrame(timeline::CameraKeyFrame::Create(Vec3(0, 1, 5), Vec3(0, 1, 0), 3.5f, Lerp::rate::Cube));

			// 紙吹雪エフェクト
			auto effectObject = AddGameObject<GameObject>();
			effectObject->GetComponent<Transform>()->SetPosition(Vec3(0, 5, 0));
			auto efkComp = effectObject->AddComponent<EfkComponent>();
			efkComp->SetEffectResource(L"Confetti");
			efkComp->PlayLoop(L"Confetti");
		}

		{
			// アニメーション終了後画面遷移を許可する
			float startTime = IsDraw ? 1.0f : 4.0f;
			timeline::TimeLine::GetInstance()->AddEvent(startTime, [&]() {m_isTransitionable = true; });
		}

		//playerの生成
		CreatePlayers(PointManager::GetInstance()->GetWinner());

		// UIアニメーション
		{
			// 1チームのメンバー数
			auto eachTeamMemberCount = int(team::TEAM_MEMBER_COUNT / 2);
			const float offsetTime[] = {
				0.6f,0.7f,0.8f
			};
			for (int i = 1; i < eachTeamMemberCount + 1; i++) {
				auto scoreUI = uiBuilder->GetUIObject(L"Red" + Util::IntToWStr(i) + L"Score");
				CreateUIAnimation(L"RedScoreUI" + Util::IntToWStr(i), scoreUI, Vec2(-400, 0), offsetTime[i - 1]);
			}

			for (int i = 1; i < eachTeamMemberCount + 1; i++) {
				auto scoreUI = uiBuilder->GetUIObject(L"Blue" + Util::IntToWStr(i) + L"Score");
				CreateUIAnimation(L"BlueScoreUI" + Util::IntToWStr(i), scoreUI, Vec2(400, 0), offsetTime[i - 1]);
			}
			{
				const wstring keys[] = {
					L"ToTitle",
					L"ToContinue",
					L"AButton",
					L"BButton"
				};
				for (auto& key : keys) {
					auto obj = uiBuilder->GetUIObject(key);
					CreateUIAnimation(key, obj, Vec2(0, -200), 0.5f);
				}
			}

			{
				const wstring keys[] = {
					L"WinOrLose",
					L"BluePoint",
					L"RedPoint",
					L"RedLabel",
					L"BlueLabel"
				};
				for (auto& key : keys) {
					auto obj = uiBuilder->GetUIObject(key);
					CreateUIAnimation(key, obj, Vec2(0, 400));
				}
			}
		}

		Online::OnlineManager::Disconnect();

		// タイムラインの再生
		timeline::TimeLine::GetInstance()->Play();
	}

	void ResultStage::CreateUIAnimation(const wstring& clipName, const shared_ptr<UIObjectBase>& uiObject, const Vec2& offset) {
		CreateUIAnimation(clipName, uiObject, offset, 0);
	}

	void ResultStage::CreateUIAnimation(const wstring& clipName, const shared_ptr<UIObjectBase>& uiObject, const Vec2& offset, float offsetTime) {
		// UIオブジェクトクリップの作成
		auto uiClip = timeline::TimeLine::GetInstance()
			->CreateClip<timeline::UIObjectClip>(clipName);

		auto rectTrans = uiObject->GetRectTransform();
		// 現在のRectTransformデータ
		auto nowRectData = RectTransformData(
			rectTrans->GetPosition(),
			rectTrans->GetScale(),
			rectTrans->GetRotation()
		);
		// 動かすUIのRectTransformをクリップにセット
		uiClip->SetTargetRectTransform(rectTrans);

		auto beforeRectData = nowRectData;
		beforeRectData.Position += offset;

		// UIの位置を開始位置に
		rectTrans->SetPosition(beforeRectData.Position);

		// 引き分けの場合はUIアニメーションをすぐに再生
		const bool IsDraw = PointManager::GetInstance()->IsDraw();
		float startTime = IsDraw ? 0.0f : 3.0f;
		startTime += offsetTime;
		uiClip->AddKeyFrame(timeline::UIObjectKeyFrame::Create(beforeRectData, startTime, Lerp::rate::Cube));
		uiClip->AddKeyFrame(timeline::UIObjectKeyFrame::Create(nowRectData, startTime + 0.5f, Lerp::rate::Cube));
	}

	void ResultStage::OnUpdate() {
		// 一通りのアニメーションが終わるまで遷移不可
		if (!m_isTransitionable)
			return;

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
	}

	void ResultStage::OnDestroy() {
		// リザルトを抜ける際にインスタンスを破棄（リセット）
		PointManager::DeleteInstance();
		TimeManager::DeleteInstance();
		ScoreManager::DeleteInstance();
	}

	void ResultStage::CreatePlayers(const team::TeamType winerType) {
		// 引き分けのとき生成しない
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

		// 位置、アニメーションの種類、遷移する秒数
		Data datas[] = {
			Data(Vec3(-1.5f, 0.1f, 0.0f), PlayerAnimationState::State::Win2,0),
			Data(Vec3(+0.0f, 0.1f, 0.0f), PlayerAnimationState::State::Win1,0.5f),
			Data(Vec3(+1.5f, 0.1f, 0.0f), PlayerAnimationState::State::Win3,1.2f),
		};

		for (auto& data : datas) {
			auto player = PlayerCreate(GetThis<Stage>());
			auto playerTrans = player->GetComponent<Transform>();
			playerTrans->SetPosition(data.position);
			auto teamMember = player->GetComponent<I_TeamMember>(false);
			teamMember->SetTeam(winerType);

			//Win2のみ歩きがあるためアニメーションを作成する
			if (data.state == PlayerAnimationState::State::Win2) {
				auto gameObjectClip = timeline::TimeLine::GetInstance()->
					CreateClip<timeline::GameObjectClip>(L"PlayerObject");
				gameObjectClip->SetTargetTransform(playerTrans);

				TransformData tData = {};
				// 歩きの始点
				tData.Position = data.position - Vec3(0, 0, 2);
				playerTrans->SetPosition(tData.Position);
				gameObjectClip->AddKeyFrame(timeline::GameObjectKeyFrame::Create(tData, 0.0f, Lerp::rate::Linear));
				// 歩きの終点
				tData.Position = data.position;
				gameObjectClip->AddKeyFrame(timeline::GameObjectKeyFrame::Create(tData, 1.3f, Lerp::rate::Linear));
			}

			// 初期値はWait
			auto animator = player->GetComponent<PlayerAnimator>();
			animator->ChangePlayerAnimation(PlayerAnimationState::State::Wait);

			// タイムラインに勝利アニメーションを始めるイベントを登録
			timeline::TimeLine::GetInstance()->AddEvent(data.time,
				[&, animator, data]() {
					animator->ChangePlayerAnimation(data.state);
				}
			);
		}
	}
}