
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

namespace basecross {

	Teleport::Teleport(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr),
		m_param(Parametor())
	{}

	void Teleport::OnLateStart() {
		auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false);
		if (!animator) {
			return;
		}

		auto exit = [&, animator]() {
			auto fadeManager = ScreenFadeManager::GetInstance();

			auto endEvent = [fadeManager]() {
				if (fadeManager) {
					fadeManager->FadeStart(FadeType::In);
				}
			};

			if (fadeManager) {
				fadeManager->FadeStart(FadeType::Out, endEvent);
			}

			transform->SetPosition(GetTeleportPosition());

			animator->ChangePlayerAnimation(PlayerAnimationState::State::Wait);	//�A�j���[�V�����̍Đ�
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

	void Teleport::OnUpdate() {

	}

	void Teleport::StartTeleport() {
		if (auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false)) {
				//�e���|�[�g�A�j���[�V����
		}

			//�G�t�F�N�g�̍Đ�
	}

	void Teleport::OpenMap() {

	}
}