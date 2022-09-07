/*!
@file SelecterSprite.cpp
@brief SelecterSprite�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "SelecterSprite.h"

#include "Maruyama/Utility/Component/SinScaling.h"

#include "Maruyama/Utility/MaruAction.h"
#include "SelectSprite.h"

#include "Maruyama/Utility/Component/SeekTarget.h"
#include "Patch/PlayerInputer.h"

#include "Maruyama/Utility/SingletonComponent/SoundManager.h"

namespace basecross {

	void SelecterSprite::OnCreate() {
		auto param = SeekTarget::Parametor();
		param.range = 0;
		param.offset = Vec3(0.0f);
		param.seekType = SeekTarget::SeekType::Lerp;
		GetGameObject()->AddComponent<SeekTarget>(nullptr, param);
	}

	void SelecterSprite::OnUpdate()
	{
		if (m_targets.size() == 0) {  //�Ώۂ�0�Ȃ珈�����s��Ȃ�
			return;
		}

		InputSelect();
		InputDecision();

		SpriteEffect();
	}

	void SelecterSprite::SpriteEffect(){
		auto sinScale = m_targets[m_selectIndex]->GetGameObject()->GetComponent<SinScaling>(false);
		if (sinScale) {
			sinScale->SetUpdateActive(true);
		}
	}

	void SelecterSprite::InputSelect()
	{
		auto& app = App::GetApp();
		auto pad = app->GetInputDevice().GetControlerVec()[0];

		if (pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_UP || pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
			EndSelect();
			auto soundManager = SoundManager::GetInstance();
			if (soundManager) {
				soundManager->PlayOneShot(SoundManager::ID::CursorSE, 0.15f);
			}
			m_selectIndex--;
		}

		if (pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_DOWN || pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
			EndSelect();
			auto soundManager = SoundManager::GetInstance();
			if (soundManager) {
				soundManager->PlayOneShot(SoundManager::ID::CursorSE, 0.15f);
			}
			m_selectIndex++;
		}

		CheckSelectIndex();

		ChangeSelect();
	}

	void SelecterSprite::InputDecision() {
		if (PlayerInputer::IsDecision()) {
			auto target = m_targets[m_selectIndex];
			target->DecisionInvoke();
		}
	}

	void SelecterSprite::EndSelect() {  
		m_sizeElapsed = 0.0f;

		auto target = m_targets[m_selectIndex];

		//scale�̒���
		auto sinScale = target->GetGameObject()->GetComponent<SinScaling>(false);
		if (sinScale) {
			sinScale->Reset();
			sinScale->SetUpdateActive(false);
		}

		//�I������
		target->SelectInvoke(false);
	}

	void SelecterSprite::ChangeSelect() {
		auto target = m_targets[m_selectIndex];

		auto seek = GetGameObject()->GetComponent<SeekTarget>(false);
		if (seek) {
			seek->SetTarget(target->GetGameObject());
		}

		//�I��
		target->SelectInvoke(true);
	}

	void SelecterSprite::CheckSelectIndex() {
		if (m_selectIndex < 0) {
			m_selectIndex = m_maxIndex;
		}

		if (m_selectIndex > m_maxIndex) {
			m_selectIndex = 0;
		}
	}

	void SelecterSprite::SetTargets(const vector<ex_weak_ptr<SelectSprite>>& targets)
	{
		m_targets.clear();
		m_selectIndex = 0;
		m_maxIndex = (int)targets.size() - 1;

		for (auto& target : targets)
		{
			m_targets.push_back(target);
		}

		if (!targets.empty()) {
			auto seek = GetGameObject()->GetComponent<SeekTarget>(false);
			if (seek) {
				seek->SetTarget(targets[0]->GetGameObject());
			}
		}
	}
}

//endbasecross