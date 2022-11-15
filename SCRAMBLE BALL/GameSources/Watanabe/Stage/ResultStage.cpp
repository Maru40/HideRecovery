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
#include "Watanabe/TimeLine/CameraTimeLine.h"
#include "Watanabe/TimeLine/GameObjectTimeLine.h"
#include "Watanabe/TimeLine/UIObjectTimeLine.h"
#include "../TimeLine/TimeLine.h"
#include "../TimeLine/ClipBase.h"
#include "../TimeLine/CameraKeyFrame.h"
#include "../TimeLine/CameraClip.h"

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

		PointManager::GetInstance()->AddPoint(team::TeamType::Blue);

		auto timeLine = AddGameObject<GameObject>()->AddComponent<timeline::CameraTimeLine>();
		m_timeLine = timeLine;

		AddGameObject<timeline::TimeLine>();

		const bool IsDraw = PointManager::GetInstance()->IsDraw();
		if (!IsDraw) {
			auto cameraClip = make_shared<timeline::CameraClip>(GetThis<ResultStage>(), L"Camera");

			cameraClip->AddKeyFrame(timeline::CameraKeyFrame::Create(Vec3(-2, 1, 2), Vec3(-3, 0.5f, 0), 0, Lerp::rate::Cube));
			cameraClip->AddKeyFrame(timeline::CameraKeyFrame::Create(Vec3(-0.5f, 1, 2), Vec3(-1.5f, 0.5f, 0), 0.5f, Lerp::rate::Cube));
			cameraClip->AddKeyFrame(timeline::CameraKeyFrame::Create(Vec3(1, 1, 2), Vec3(0, 0.5f, 0), 1.5f, Lerp::rate::Cube));
			cameraClip->AddKeyFrame(timeline::CameraKeyFrame::Create(Vec3(2.5f, 1, 2), Vec3(1.5f, 0.5f, 0), 2.5f, Lerp::rate::Cube));
			cameraClip->AddKeyFrame(timeline::CameraKeyFrame::Create(Vec3(0, 1, 5), Vec3(0, 1, 0), 3.5f, Lerp::rate::Cube));
			timeLine->AddEvent(4.0f, [&]() {m_isTransitionable = true; });

			timeline::TimeLine::GetInstance()->AddClip(cameraClip);
			timeline::TimeLine::GetInstance()->Play();

			// 引き分け以外に生成
			// 紙吹雪エフェクト
			auto effectObject = AddGameObject<GameObject>();
			effectObject->GetComponent<Transform>()->SetPosition(Vec3(0, 5, 0));
			auto efkComp = effectObject->AddComponent<EfkComponent>();
			efkComp->SetEffectResource(L"Confetti");
			efkComp->PlayLoop(L"Confetti");
		}
		else {
			timeLine->AddEvent(0.5f, [&]() {m_isTransitionable = true; });
		}

		//playerの生成
		CreatePlayers(PointManager::GetInstance()->GetWinner());

		// UIアニメーション
		{
			// 1チームのメンバー数
			auto eachTeamMemberCount = int(team::TEAM_MEMBER_COUNT / 2);
			for (int i = 1; i < eachTeamMemberCount + 1; i++) {
				auto scoreUI = uiBuilder->GetUIObject(L"Red" + Util::IntToWStr(i) + L"Score");
				CreateUIAnimation(scoreUI, Vec2(-400, 0));
			}

			for (int i = 1; i < eachTeamMemberCount + 1; i++) {
				auto scoreUI = uiBuilder->GetUIObject(L"Blue" + Util::IntToWStr(i) + L"Score");
				CreateUIAnimation(scoreUI, Vec2(400, 0));
			}

			{
				auto obj = uiBuilder->GetUIObject(L"ToTitle");
				CreateUIAnimation(obj, Vec2(0, -200));
			}
			{
				auto obj = uiBuilder->GetUIObject(L"ToContinue");
				CreateUIAnimation(obj, Vec2(0, -200));
			}
			{
				auto obj = uiBuilder->GetUIObject(L"AButton");
				CreateUIAnimation(obj, Vec2(0, -200));
			}
			{
				auto obj = uiBuilder->GetUIObject(L"BButton");
				CreateUIAnimation(obj, Vec2(0, -200));
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
					CreateUIAnimation(obj, Vec2(0, 400));
				}
			}
		}

		Online::OnlineManager::Disconnect();

		timeLine->Play();
	}

	void ResultStage::CreateUIAnimation(const shared_ptr<UIObjectBase>& uiObject, const Vec2& offset) {
		auto uiTimeLine = uiObject->AddComponent<timeline::UIObjectTimeLine>();
		auto nowRectTrans = uiObject->GetRectTransform();
		auto nowRectData = RectTransformData(
			nowRectTrans->GetPosition(),
			nowRectTrans->GetScale(),
			nowRectTrans->GetRotation()
		);

		auto beforeRectData = nowRectData;
		beforeRectData.Position += offset;

		nowRectTrans->SetPosition(beforeRectData.Position);

		uiTimeLine->AddKeyFrame(UIObjectKeyFrameData(beforeRectData, 0, Lerp::rate::Cube));
		uiTimeLine->AddKeyFrame(UIObjectKeyFrameData(nowRectData, 0.5f, Lerp::rate::Cube));

		const bool IsDraw = PointManager::GetInstance()->IsDraw();
		m_timeLine.lock()->AddEvent(IsDraw ? 0.0f : 3.5f,
			[&, uiTimeLine]() {
				uiTimeLine->Play();
			}
		);
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

			if (data.state == PlayerAnimationState::State::Win2) {
				auto gameObjectTimeLine = player->AddComponent<timeline::GameObjectTimeLine>();

				TransformData tData = {};
				tData.Position = data.position - Vec3(0, 0, 2);
				playerTrans->SetPosition(tData.Position);
				gameObjectTimeLine->AddKeyFrame(GameObjectKeyFrameData(tData, 0.0f, Lerp::rate::Linear));
				tData.Position = data.position;
				gameObjectTimeLine->AddKeyFrame(GameObjectKeyFrameData(tData, 1.3f, Lerp::rate::Linear));

				gameObjectTimeLine->Play();
			}

			auto animator = player->GetComponent<PlayerAnimator>(false);
			animator->ChangePlayerAnimation(PlayerAnimationState::State::Wait);

			// カメラタイムラインに勝利アニメーションを始めるイベントを登録
			if (auto timeLine = m_timeLine.lock()) {
				auto cameraTimeLine = timeLine;// ->GetComponent<CameraTimeLine>();
				cameraTimeLine->AddEvent(data.time,
					[&, animator, data]() {
						animator->ChangePlayerAnimation(data.state);
					}
				);
			}
		}
	}
}