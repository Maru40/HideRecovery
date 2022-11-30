
/*!
@file Teleport.cpp
@brief Teleportクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Teleport.h"

#include "Watanabe/Component/PlayerAnimator.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/Utility/SingletonComponent/ScreenFadeManager.h"
#include "Maruyama/Utility/Component/FadeSprite.h"

#include "Maruyama/UI/2D/Component/FieldMap.h"

#include "Watanabe/DebugClass/Debug.h"
#include "Patch/PlayerInputer.h"
#include "Maruyama/UI/2D/Object/SpriteObject.h"
#include "Maruyama/UI/2D/Component/MapCursor.h"

#include "Maruyama/Utility/Timer/TimeHelper.h"
#include "Maruyama/Utility/Timer/GameTimer.h"

#include "Patch/CameraHelper.h"
#include "Maruyama/Camera/Component/CameraForwardController.h"
#include "Maruyama/Utility/Component/ToTargetMove.h"
#include "Maruyama/Camera/Component/LookAtCameraManager.h"
#include "Patch/SpringArmComponent.h"
#include "Maruyama/Player/Object/PlayerObject.h"

#include "Watanabe/Effekseer/EfkComponent.h"

#include "Maruyama/Interface/I_TeamMember.h"
#include "Maruyama/Player/Component/Teleport.h"

#include "Watanabe/DebugClass/Debug.h"
#include "OwnArea.h"
#include "Maruyama/Utility/SingletonComponent/GameManager.h"

#include "Maruyama/Utility/Utility.h"
#include "Maruyama/Utility/UtilityObstacle.h"
#include "Maruyama/UI/2D/Component/TeleportUI.h"

#include "Maruyama/Utility/MaruHitHelper.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// テレポート機能のパラメータ
	//--------------------------------------------------------------------------------------

	Teleport_Parametor::Teleport_Parametor() :
		maxRangeLate(0.5f),
		position(Vec3(0.0f)),
		isTeleporting(false)
	{}

	//--------------------------------------------------------------------------------------
	/// テレポート本体
	//--------------------------------------------------------------------------------------

	Teleport::Teleport(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr),
		m_param(Parametor()),
		m_timer(new GameTimer(0)),
		m_teleportSoundClip(L"TeleportSE", false, 0.25f)
	{}

	void Teleport::OnCreate() {
	}

	void Teleport::OnLateStart() {
		m_teamMember = GetGameObject()->GetComponent<I_TeamMember>(false);
		m_soundEmmiter = GetGameObject()->GetComponent<SoundEmitter>(false);

		SettingFieldMap();			//マップテクスチャの設定
		SettingAnimationEvent();	//アニメーションイベント設定
	}

	void Teleport::OnUpdate() {
		m_timer->UpdateTimer();

		if (!m_teleportUI.lock()) {
			m_teleportUI = maru::Utility::FindComponent<TeleportUI>(GetStage());
		}
	}

	void Teleport::SettingFieldMap() {
		auto fieldMap = FieldMap::GetInstance();
		if (!fieldMap) {
			fieldMap = GetStage()->AddGameObject<GameObject>()->AddComponent<FieldMap>();
		}

		fieldMap->GetMapCursor()->SetTarget(GetGameObject());
		fieldMap->SetMapDraw(false);

		m_fieldMap = fieldMap;
	}

	void Teleport::SettingAnimationEvent() {
		auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false);
		if (!animator) {
			return;
		}

		auto start = [&]() {

		};

		//アニメーション終了時に呼ぶイベント
		std::weak_ptr<PlayerAnimator> weakAnimator = animator;
		auto exit = [&, weakAnimator]() {
			auto animator = weakAnimator.lock();

			const bool IsFade = false;
			if (IsFade) {
				//フェード終了イベント
				auto endEvent = [&, weakAnimator]() {
					auto fadeManager = ScreenFadeManager::GetInstance(GetStage());
					weakAnimator.lock()->ChangePlayerAnimation(PlayerAnimationState::State::EndTeleport);
					GetGameObject()->GetComponent<Transform>()->SetPosition(GetTeleportPosition());	//テレポート
					
					if (fadeManager) {
						fadeManager->FadeStart(FadeType::In);
					}
				};

				auto fadeManager = ScreenFadeManager::GetInstance(GetStage());
				//フェード開始イベント
				if (fadeManager) {
					fadeManager->FadeStart(FadeType::Out, endEvent);
				}
			}
			else {

				auto camera = m_camera.lock();

				if (camera)
				{
					//カメラを今のカメラに合わせる。
					auto& tpsCamera = GetStage()->GetView()->GetTargetCamera();
					auto tpsCameraTrans = tpsCamera->GetCameraObject()->GetComponent<Transform>();
					auto tpsAt = tpsCamera->GetAt();
					auto tpsForward = tpsAt - tpsCamera->GetEye();

					auto cameraTrans = camera->GetGameObject()->GetComponent<Transform>();
					cameraTrans->SetPosition(tpsCameraTrans->GetPosition());
					auto forwardController = camera->GetGameObject()->GetComponent<CameraForwardController>(false);
					if (forwardController) {
						forwardController->SetDirection(tpsForward);
					}
					camera->SetUpdateActive(true);
				}

				//移動しきったら、演出開始
				auto moveEndEvent = [&, weakAnimator]() {
					auto animator = weakAnimator.lock();
					weakAnimator.lock()->ChangePlayerAnimation(PlayerAnimationState::State::EndTeleport);

					//当たり判定復活
					if (auto collision = GetGameObject()->GetComponent<CollisionObb>(false)) {
						collision->SetUpdateActive(true);
					}

					//重力復活
					if (auto gravity = GetGameObject()->GetComponent<Gravity>(false)) {
						gravity->SetUpdateActive(true);
					}

					GetGameObject()->GetComponent<Transform>()->SetPosition(GetTeleportPosition());	//テレポート

					auto playerObject = dynamic_pointer_cast<PlayerObject>(GetGameObject());
					auto springArm = playerObject->GetArm()->GetComponent<SpringArmComponent>();
					springArm->GetGameObject()->GetComponent<Transform>()->SetPosition(GetTeleportPosition());
					springArm->SetCurrentArmRange(springArm->GetArmRange());
					springArm->OnUpdate2();

					auto camera = m_camera.lock();

					if (camera)
					{
						camera->SetUpdateActive(false);
					}

					//エフェクトの再生
					if (auto efkComp = GetGameObject()->GetComponent<EfkComponent>(false)) {
						efkComp->Play(L"Respawn");
					}

					//表示
					GetGameObject()->SetDrawActive(true);

					//音の再生
					if (auto soundEmitter = m_soundEmmiter.lock()) {
						soundEmitter->PlaySoundClip(m_teleportSoundClip);
					}
				};

				//カメラを移動させる
				auto mover = m_toTargetMove.lock();
				if (mover) {
					mover->MoveStart(m_cameraPosition, moveEndEvent);
				}

			}

			//エフェクトの再生
			if (auto efkComp = GetGameObject()->GetComponent<EfkComponent>(false)) {
				efkComp->Play(L"Respawn");
			}

			//非表示
			GetGameObject()->SetDrawActive(false);

			//重力解除
			if (auto gravity = GetGameObject()->GetComponent<Gravity>(false)) {
				gravity->SetUpdateActive(false);
			}

			//当たり判定解除
			if (auto collision = GetGameObject()->GetComponent<CollisionObb>(false)) {
				collision->SetUpdateActive(false);
			}

			//音の再生
			if (auto soundEmitter = m_soundEmmiter.lock()) {
				soundEmitter->PlaySoundClip(m_teleportSoundClip);
			}
		};

		//アニメーションイベントの登録
		animator->AddAnimationEvent(
			PlayerAnimationState::State::StartTeleport,
			start,
			nullptr,
			exit
		);

		//テレポート終了アニメーションイベント
		animator->AddAnimationEvent(
			PlayerAnimationState::State::EndTeleport,
			nullptr,
			nullptr,
			[&]() { 
				m_param.isTeleporting = false; 
			}
		);
	}

	void Teleport::StartTeleport() {
		StartTeleport(GetFieldMap()->GetMapCursor()->GetCursorFiledPosition());
	}

	void Teleport::StartTeleport(const Vec3& teleportPosition) {
		//障害物がテレポート先にあるならテレポートできない
		auto underTeleportPosition = teleportPosition;
		auto upperTeleportPosition = teleportPosition;
		underTeleportPosition.y += 1.0f;
		upperTeleportPosition.y += 10.0f;
		if (maru::UtilityObstacle::IsRayObstacle(
			upperTeleportPosition,
			underTeleportPosition,
			GetStage()->GetGameObjectVec()
		)) {
			//ダメな音を出す。
			return;
		}

		//テレポート場所を設定
		SetTeleportPosition(teleportPosition);

		//マップを閉じる
		CloseMap();

		if (auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false)) {
			animator->ChangePlayerAnimation(PlayerAnimationState::State::StartTeleport);	//テレポートアニメーション
		}

		m_param.isTeleporting = true;
	}

	void Teleport::OpenMap() {
		auto fieldMap = GetFieldMap();
		if (fieldMap->GetMapDraw()) {	//マップが開いているならCloseする。
			CloseMap();
			return;
		}

		fieldMap->GetMapCursor()->SetTarget(GetGameObject());
		fieldMap->SetMapDraw(true);
	}

	void Teleport::CloseMap() {
		auto fieldMap = GetFieldMap();

		fieldMap->SetMapDraw(false);
	}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
	//--------------------------------------------------------------------------------------

	std::shared_ptr<FieldMap> Teleport::GetFieldMap() const {
		return m_fieldMap.lock();
	}

	bool Teleport::CanTeleport() const {
		auto gameManager = GameManager::GetInstance();
		if (gameManager && gameManager->GetCurrentState() != GameManager::State::Game) {
			return false;	//ゲーム状態でないなら、テレポートできない
		}

		auto teamMember = GetGameObject()->GetComponent<I_TeamMember>(false);
		if (teamMember && !teamMember->IsInArea()) {
			return false;
		}

		//距離がありすぎるならテレポートしない
		auto teleportPosition = GetFieldMap()->GetMapCursor()->GetCursorFiledPosition();
		auto ownArea = teamMember->GetOwnArea();
		auto toTeleportPosition = teleportPosition - ownArea->GetGameObject()->GetComponent<Transform>()->GetPosition();
		auto centerPosition = ownArea->GetGameObject()->GetComponent<Transform>()->GetPosition();
		auto fieldMap = FieldMap::GetInstance();
		auto maxRange = GetFieldMap()->GetDepth() * m_param.maxRangeLate;	//Rect
		auto rect = maru::Rect(centerPosition, fieldMap->GetWidth(), maxRange * 2.0f);
		if (!rect.IsInRect(teleportPosition)) {
			return false;
		}

		return GetFieldMap()->IsMapDraw();	//現在はマップが開いているなら飛べる。
	}

	bool Teleport::IsTeleporting() const {
		return m_param.isTeleporting;
	}

	void Teleport::SetTeleportCamera(const std::shared_ptr<VirtualCamera> camera) {
		m_camera = camera;
	}

	std::shared_ptr<VirtualCamera> Teleport::GetTeleportCamera() const {
		return m_camera.lock();
	}

	std::shared_ptr<TeleportUI> Teleport::GetTeleportUI() const {
		return m_teleportUI.lock();
	}


	//if (PlayerInputer::GetInstance()->IsRightDown()) {
		//	auto param = Builder::VertexPCTParametor(Vec3(100.0f, 100.0f, 0.0f), Vec2(256.0f, 256.0f), L"Point_TX");
		//	auto sprite = GetStage()->AddGameObject<SpriteObject>(param);
		//	auto spriteTrans = sprite->GetComponent<Transform>();

		//	auto rect = GetFieldMap()->GetRect();
		//	auto startPosition = transform->GetPosition();
		//	float xRate = startPosition.x / (rect.width * 0.5f);
		//	float yRate = startPosition.z / (rect.depth * 0.5f);
		//	spriteTrans->SetPosition(Vec3(256.0f * xRate, 512.0f * yRate, 0.0f));
	//}
}