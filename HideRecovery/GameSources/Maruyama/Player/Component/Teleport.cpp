
/*!
@file Teleport.cpp
@brief Teleport�N���X����
�S���F�ێR�T��
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

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �e���|�[�g�{��
	//--------------------------------------------------------------------------------------

	Teleport::Teleport(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr),
		m_param(Parametor()),
		m_timer(new GameTimer(0))
	{}

	void Teleport::OnCreate() {
		//�J�����̐���
		auto cameraObject = GetStage()->AddGameObject<GameObject>();
		auto camera = cameraObject->AddComponent<VirtualCamera>(11);
		camera->SetUpdateActive(false);
		cameraObject->AddComponent<CameraForwardController>(camera);
		auto mover = cameraObject->AddComponent<ToTargetMove>();
		//mover->SetUpdateActive(false);

		m_camera = camera;
	}

	void Teleport::OnLateStart() {
		SettingFieldMap();			//�}�b�v�e�N�X�`���̐ݒ�
		SettingAnimationEvent();	//�A�j���[�V�����C�x���g�ݒ�
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

		//�A�j���[�V�����I�����ɌĂԃC�x���g
		auto exit = [&, animator]() {
			auto fadeManager = ScreenFadeManager::GetInstance(GetStage());

			const bool IsFade = false;
			if (IsFade) {
				//�t�F�[�h�I���C�x���g
				auto endEvent = [&, fadeManager, animator]() {
					animator->ChangePlayerAnimation(PlayerAnimationState::State::EndTeleport);
					GetGameObject()->GetComponent<Transform>()->SetPosition(GetTeleportPosition());	//�e���|�[�g
					
					if (fadeManager) {
						fadeManager->FadeStart(FadeType::In);
					}
				};

				//�t�F�[�h�J�n�C�x���g
				if (fadeManager) {
					fadeManager->FadeStart(FadeType::Out, endEvent);
				}
			}
			else {
				//�J���������̃J�����ɍ��킹��B
				auto tpsCamera = GetStage()->GetView()->GetTargetCamera();
				auto tpsCameraTrans = tpsCamera->GetCameraObject()->GetComponent<Transform>();
				auto tpsAt = tpsCamera->GetAt();
				auto tpsForward = tpsAt - tpsCamera->GetEye();

				auto camera = m_camera.lock();
				auto cameraTrans = camera->GetGameObject()->GetComponent<Transform>();
				cameraTrans->SetPosition(tpsCameraTrans->GetPosition());
				auto forwardController = camera->GetGameObject()->GetComponent<CameraForwardController>(false);
				if (forwardController) {
					forwardController->SetDirection(tpsForward);
				}
				camera->SetUpdateActive(true);

				//�ړ�����������A���o�J�n
				auto moveEndEvent = [&, animator, tpsCamera, tpsCameraTrans, cameraTrans]() {
					animator->ChangePlayerAnimation(PlayerAnimationState::State::EndTeleport);
					GetGameObject()->GetComponent<Transform>()->SetPosition(GetTeleportPosition());	//�e���|�[�g
					
					//tpsCameraTrans->SetPosition(cameraTrans->GetPosition());
					//tpsCamera->SetAt(GetTeleportPosition());
					//tpsCamera->SetEye(cameraTrans->GetPosition());
					//auto tpsCameraObject = tpsCamera->GetCameraObject();
					auto playerObject = dynamic_pointer_cast<PlayerObject>(GetGameObject());
					auto springArm = playerObject->GetArm()->GetComponent<SpringArmComponent>();
					springArm->GetGameObject()->GetComponent<Transform>()->SetPosition(GetTeleportPosition());
					springArm->SetCurrentArmRange(springArm->GetArmRange());

					m_camera.lock()->SetUpdateActive(false);
				};

				//�J�������ړ�������
				auto mover = camera->GetGameObject()->GetComponent<ToTargetMove>(false);
				if (mover) {
					auto position = GetTeleportPosition();
					position += -tpsForward;

					mover->MoveStart(position, moveEndEvent);
				}
			}


				//�G�t�F�N�g�̍Đ�
		};

		//�A�j���[�V�����C�x���g�̓o�^
		animator->AddAnimationEvent(
			PlayerAnimationState::State::StartTeleport,
			nullptr,
			nullptr,
			exit
		);
	}

	void Teleport::StartTeleport() {
		//�e���|�[�g�ꏊ��ݒ�
		SetTeleportPosition(GetFieldMap()->GetMapCursor()->GetCursorFiledPosition());

		//�}�b�v�����
		CloseMap();

		if (auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false)) {
			animator->ChangePlayerAnimation(PlayerAnimationState::State::StartTeleport);	//�e���|�[�g�A�j���[�V����
		}

			//�G�t�F�N�g�̍Đ�
	}

	void Teleport::OpenMap() {
		auto fieldMap = GetFieldMap();
		if (fieldMap->GetMapDraw()) {	//�}�b�v���J���Ă���Ȃ�Close����B
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
	/// �A�N�Z�b�T
	//--------------------------------------------------------------------------------------

	std::shared_ptr<FieldMap> Teleport::GetFieldMap() const {
		return m_fieldMap.lock();
	}

	bool Teleport::IsTeleport() const {
		return GetFieldMap()->IsMapDraw();	//���݂̓}�b�v���J���Ă���Ȃ��ׂ�B
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