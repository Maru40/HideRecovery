/*!
@file PlayerAnimationCtrl.cpp
@brief PlayerAnimationCtrl�N���X����
�S���F�ێR�T��
*/


#include "stdafx.h"
#include "Project.h"

#include "PlayerAnimationCtrl.h"

#include "VelocityManager.h"

namespace basecross {

	PlayerAnimationCtrl::PlayerAnimationCtrl(const std::shared_ptr<GameObject>& objPtr):
		AnimationCtrl(objPtr), m_currentState(State::Wait)
	{}

	void PlayerAnimationCtrl::OnCreate() 
	{
		auto object = GetGameObject();

		//���b�V���̐���
		auto drawComp = object->GetComponent<PNTBoneModelDraw>();

		//�A�j���[�V�����p�̃p�����[�^
		AddAnimeParam<PlayerAnimationCtrl> params[] = {
			{State::Wait,	L"Wait",       1,    39,   true, &PlayerAnimationCtrl::Wait},
			{State::Walk,	L"Dash",       99,  119,  true, &PlayerAnimationCtrl::Walk},

			//{L"Jump",       53,   65,  true, &PlayerAnimationCtrl::Jump},
			//{L"Turn",       66,  105,  true, &PlayerAnimationCtrl::Turn},
			//{L"HundShake", 106,  150,  true, &PlayerAnimationCtrl::HundShake},
			//{L"LookOut",   151,  250,  true, &PlayerAnimationCtrl::LookOut},
			//{L"Appeal",    251,  300,  true, &PlayerAnimationCtrl::Appeal},
			//{L"Fall",      301,  309,  true, &PlayerAnimationCtrl::Fall},
			//{L"Landing",   310,  320,  true, &PlayerAnimationCtrl::Landing},  //���n
			//{L"Dest",      321,  370,  true, &PlayerAnimationCtrl::Dest},
			//{L"TurnFixed",  75,   75,  true, &PlayerAnimationCtrl::TurnFixed},
		};

		//�A�j���[�V�����̐ݒ�
		for (auto& param : params)
		{
			drawComp->AddAnimation(
				param.stateName, 
				param.startTime, 
				param.timeLength, 
				param.loop, 
				param.timeParSecond
			);

			m_stateStringMap[param.stateType] = param.stateName;	//�}�b�v�ɓo�^
			SetAnimaiton(param.stateName, param.func);				//�A�j���[�V�����̓o�^
		}

		ChangeAnimation(params[0].stateName);  //�����A�j���\�V�����ݒ�
	}


	void PlayerAnimationCtrl::OnUpdate()
	{
		auto draw = GetGameObject()->GetComponent<SmBaseDraw>();
		auto name = draw->GetCurrentAnimation();

		m_animations[name](*(this));

		//�ȈՑJ�ڕ�
		if (auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false)) {
			constexpr float DashSpeed = 2.0f;
			if (velocityManager->GetVelocity().length() < DashSpeed) {
				ChangeAnimation(L"Wait");
			}
			else {
				ChangeAnimation(L"Dash");
			}
		}

		return;
		//�A�j���[�V�����m�F�p
		auto pad = App::GetApp()->GetInputDevice().GetControlerVec()[0];
		if (pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_UP) {
			//ChangeAnimation(L"Turn");
		}

		if (pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
			//ChangeAnimation(L"HundShake");
		}

		if (pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
			//ChangeAnimation(L"LookOut");
			ChangeAnimation(L"HundShake");
		}

		if (pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
			ChangeAnimation(L"Appeal");
		}

		if (pad.wPressedButtons & XINPUT_GAMEPAD_LEFT_THUMB) {
			//ChangeAnimation(L"Fall");
		}

		if (pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
			ChangeAnimation(L"LookOut");
		}
	}

	void PlayerAnimationCtrl::Wait()
	{
		auto delta = App::GetApp()->GetElapsedTime();
		float speed = 15.0f;

		auto draw = GetGameObject()->GetComponent<SmBaseDraw>();
		draw->UpdateAnimation(delta * speed);
	}

	void PlayerAnimationCtrl::Walk()
	{
		auto velocityMananger = GetGameObject()->GetComponent<VelocityManager>(false);

		const float speed = 2.0f;
		auto delta = App::GetApp()->GetElapsedTime();

		auto moveVec = velocityMananger->GetVelocity();
		
		auto draw = GetGameObject()->GetComponent<SmBaseDraw>();

		//�^�C����0�Ŗ����A���AmoveVec��0.0f��������B
		if (draw->GetCurrentAnimationTime() != 0.0f && moveVec.length() == 0.0f) { 
			draw->UpdateAnimation(delta * speed);

			if (draw->GetCurrentAnimationTime() == 0.0f) {
				//ChangeAnimation(L"ReturnWait");
			}
		}
		else {
			draw->UpdateAnimation(delta * moveVec.length() * speed);
		}
	}

	void PlayerAnimationCtrl::DefaultPlay(const float speed) {
		auto delta = App::GetApp()->GetElapsedTime();

		auto draw = GetGameObject()->GetComponent<SmBaseDraw>();
		draw->UpdateAnimation(delta * speed);

		if (draw->GetCurrentAnimationTime() == 0.0f) {
			ChangeAnimation(L"Wait");
		}
	}

	void PlayerAnimationCtrl::SetAnimaiton(const wstring& animeName, const function<void(PlayerAnimationCtrl&)> func) {
		m_animations[animeName] = func;
	}

	void PlayerAnimationCtrl::ChangeAnimation(const wstring& animeName) {
		auto draw = GetGameObject()->GetComponent<SmBaseDraw>();
		auto nowName = draw->GetCurrentAnimation();

		//�����A�j���[�V������������
		if (nowName == animeName) {  
			return;  //�Đ�����A�j���[�V�����������̏ꍇ�X�V���Ȃ��B
		}

		draw->ChangeCurrentAnimation(animeName);
	}

}

//endbasecross