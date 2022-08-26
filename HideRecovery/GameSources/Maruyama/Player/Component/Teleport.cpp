
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

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �e���|�[�g�{��
	//--------------------------------------------------------------------------------------

	Teleport::Teleport(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr),
		m_param(Parametor())
	{}

	void Teleport::OnLateStart() {
		SettingFieldMap();			//�}�b�v�e�N�X�`���̐ݒ�
		//SettingAnimationEvent();	//�A�j���[�V�����C�x���g�ݒ�
	}

	void Teleport::OnUpdate() {
		//Debug::GetInstance()->Log(transform->GetPosition());
		if (PlayerInputer::GetInstance()->IsRightDown()) {
			auto param = Builder::VertexPCTParametor(Vec3(100.0f, 100.0f, 0.0f), Vec2(256.0f, 256.0f), L"Point_TX");
			auto sprite = GetStage()->AddGameObject<SpriteObject>(param);
			auto spriteTrans = sprite->GetComponent<Transform>();

			auto rect = GetFieldMap()->GetRect();
			auto startPosition = transform->GetPosition();
			float xRate = startPosition.x / (rect.width * 0.5f);
			float yRate = startPosition.z / (rect.depth * 0.5f);
			spriteTrans->SetPosition(Vec3(256.0f * xRate, 512.0f * yRate, 0.0f));
		}

		if (PlayerInputer::GetInstance()->IsLeftDown()) {
			//GetFieldMap()->SetMapDraw(GetF);
		}
	}

	void Teleport::SettingFieldMap() {
		auto fieldMap = FieldMap::GetInstance();
		fieldMap->SetMapDraw(false);

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

			//�t�F�[�h�I���C�x���g
			auto endEvent = [fadeManager]() {
				if (fadeManager) {
					fadeManager->FadeStart(FadeType::In);
				}
			};

			//�t�F�[�h�J�n�C�x���g
			if (fadeManager) {
				fadeManager->FadeStart(FadeType::Out, endEvent);
			}

			transform->SetPosition(GetTeleportPosition());	//�e���|�[�g

			animator->ChangePlayerAnimation(PlayerAnimationState::State::Wait);	//�A�j���[�V�����̍Đ�(�����I�ɕύX)
				//�G�t�F�N�g�̍Đ�
		};

		//�A�j���[�V�����C�x���g�̓o�^
		animator->AddAnimationEvent(
			PlayerAnimationState::State::Dash,
			nullptr,
			nullptr,
			exit
		);
	}

	void Teleport::StartTeleport() {
		if (auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false)) {
				//�e���|�[�g�A�j���[�V����
		}

			//�G�t�F�N�g�̍Đ�
	}

	void Teleport::OpenMap() {
		GetFieldMap()->SetMapDraw(true);
	}

	//--------------------------------------------------------------------------------------
	/// �A�N�Z�b�T
	//--------------------------------------------------------------------------------------

	std::shared_ptr<FieldMap> Teleport::GetFieldMap() const {
		return m_fieldMap.lock();
	}
}