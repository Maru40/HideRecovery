
/*!
@file Teleport.cpp
@brief Teleportクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Teleport.h"

#include "Watanabe/Component/PlayerAnimator.h"

#include "SingletonComponent.h"
#include "ScreenFadeManager.h"
#include "FadeSprite.h"

#include "FieldMap.h"

#include "Watanabe/DebugClass/Debug.h"
#include "PlayerInputer.h"
#include "SpriteObject.h"
#include "MapCursor.h"

#include "TimeHelper.h"
#include "GameTimer.h"

#include "CameraHelper.h"
#include "Maruyama/Camera/Component/CameraForwardController.h"
#include "ToTargetMove.h"
#include "LookAtCameraManager.h"
#include "SpringArmComponent.h"
#include "PlayerObject.h"

#include "Watanabe/Effekseer/EfkComponent.h"

#include "Maruyama/Interface/I_TeamMember.h"

#include "Watanabe/DebugClass/Debug.h"

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
		m_timer(new GameTimer(0))
	{}

	void Teleport::OnCreate() {
	}

	void Teleport::OnLateStart() {
		SettingFieldMap();			//マップテクスチャの設定
		SettingAnimationEvent();	//アニメーションイベント設定
	}

	void Teleport::OnUpdate() {
		m_timer->UpdateTimer();
	}

	void Teleport::SettingFieldMap() {
		auto fieldMap = FieldMap::GetInstance();
		fieldMap->SetMapDraw(false);
		fieldMap->GetMapCursor()->SetTarget(GetGameObject());

		m_fieldMap = fieldMap;
	}

	void Teleport::SettingAnimationEvent() {
		auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false);
		if (!animator) {
			return;
		}

		//アニメーション終了時に呼ぶイベント
		auto exit = [&, animator]() {
			auto fadeManager = ScreenFadeManager::GetInstance(GetStage());

			const bool IsFade = false;
			if (IsFade) {
				//フェード終了イベント
				auto endEvent = [&, fadeManager, animator]() {
					animator->ChangePlayerAnimation(PlayerAnimationState::State::EndTeleport);
					GetGameObject()->GetComponent<Transform>()->SetPosition(GetTeleportPosition());	//テレポート
					
					if (fadeManager) {
						fadeManager->FadeStart(FadeType::In);
					}
				};

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
				auto moveEndEvent = [&, animator]() {
					animator->ChangePlayerAnimation(PlayerAnimationState::State::EndTeleport);
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

					//当たり判定復活
					if (auto collision = GetGameObject()->GetComponent<CollisionObb>(false)) {
						collision->SetUpdateActive(true);
					}

					//重力復活
					if (auto gravity = GetGameObject()->GetComponent<Gravity>(false)) {
						gravity->SetUpdateActive(true);
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

			//当たり判定解除
			if (auto collision = GetGameObject()->GetComponent<CollisionObb>(false)) {
				collision->SetUpdateActive(false);
			}

			//重力解除
			if (auto gravity = GetGameObject()->GetComponent<Gravity>(false)) {
				gravity->SetUpdateActive(false);
			}
		};

		//アニメーションイベントの登録
		animator->AddAnimationEvent(
			PlayerAnimationState::State::StartTeleport,
			nullptr,
			nullptr,
			exit
		);

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
		auto teamMember = GetGameObject()->GetComponent<I_TeamMember>(false);
		if (teamMember && !teamMember->IsInArea()) {
			return false;
		}

		//距離がありすぎるならテレポートしない
		auto teleportPosition = GetFieldMap()->GetMapCursor()->GetCursorFiledPosition();
		auto toTeleportPosition = teleportPosition - transform->GetPosition();
		auto maxRange = GetFieldMap()->GetRect().depth * m_param.maxRangeLate;
		if (toTeleportPosition.length() > maxRange) {
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