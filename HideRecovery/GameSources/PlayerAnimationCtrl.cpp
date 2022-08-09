/*!
@file PlayerAnimationCtrl.cpp
@brief PlayerAnimationCtrl�N���X����
�S���F�ێR�T��
*/


#include "stdafx.h"
#include "Project.h"

#include "PlayerAnimationCtrl.h"

#include "VelocityManager.h"
//#include "CMovePlayer.h"
//#include "FunctionTimer.h"
//
//#include "DebugSprite.h"
//#include "AccessAddFun.h"
//#include "Lock.h"
//#include "BillBoard.h"

namespace basecross {

	float PlayerAnimationCtrl::CalucGravityVelocityY() {
		//�R���|�[�l���g�����݂��邩�̊m�F
		//auto jump = GetGameObject()->GetComponent<JumpCtrl>(false);
		//if (!jump) {
		//	return 0.0f;
		//}
		//auto gravi = GetGameObject()->GetComponent<Gravity>(false);
		//if (!gravi) {
		//	return 0.0f;
		//}

		//float jumpVec = jump->GetJumpVelocity();
		//auto graviVec = gravi->GetGravityVelocity();

		//auto velocity = jumpVec + graviVec.y;

		//return velocity;
		return 0;
	}

	bool PlayerAnimationCtrl::IsJump() {
		//auto gravi = GetGameObject()->GetComponent<Gravity>(false);
		//if (!gravi) {
		//	return false;
		//}

		//auto velocity = gravi->GetGravityVelocity();

		//if (velocity.y == 0.0f) {
		//	return false;  //0.0f�����ɏd�͂����������Ȃ� == �W�����v���łȂ�
		//}
		//else {
		//	return true;  //�W�����v��
		//}
		return false;
	}


	void PlayerAnimationCtrl::OnCreate() 
	{
		auto obj = GetGameObject();

		//���b�V���̒����pMat
		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(1.0f),
			Vec3(0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, -1.0f, 0.0f)
		);

		//���b�V���̐���
		auto drawComp = obj->GetComponent<PNTBoneModelDraw>();
		//drawComp->SetMeshResource(L"Player_Mesh");
		//drawComp->SetMeshToTransformMatrix(spanMat);
		//auto shadow = obj->AddComponent<Shadowmap>();
		//shadow->SetMeshResource(L"Player_Mesh");

		//�A�j���[�V�����p�̃p�����[�^
		AddAnimeParam<PlayerAnimationCtrl> params[] = {
			{L"Wait",       10,    40,  true, &PlayerAnimationCtrl::Wait},
			{L"Dash",       100,  119,  true, &PlayerAnimationCtrl::Walk},
			{L"ReturnWait", 43,   45,  true, &PlayerAnimationCtrl::ReturnWalk},

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

		for (auto& param : params)
		{
			drawComp->AddAnimation(
				param.name, 
				param.startTime, 
				param.timeLength, 
				param.loop, 
				param.timeParSecond);

			SetAnimaiton(param.name, param.func);
		}

		ChangeAnimation(params[0].name);  //�����A�j���\�V�����ݒ�
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
		float speed = 1.0f;

		auto draw = GetGameObject()->GetComponent<SmBaseDraw>();
		draw->UpdateAnimation(delta * speed);
	}

	void PlayerAnimationCtrl::Walk()
	{
		auto obj = GetGameObject();

		auto velocityMananger = GetGameObject()->GetComponent<VelocityManager>(false);

		float speed = 2.0f;
		auto delta = App::GetApp()->GetElapsedTime();

		auto moveVec = velocityMananger->GetVelocity();
		
		auto draw = GetGameObject()->GetComponent<SmBaseDraw>();

		//�^�C����0�Ŗ����A���AmoveVec��0.0f��������B
		if (draw->GetCurrentAnimationTime() != 0.0f && moveVec.length() == 0.0f) { 
			draw->UpdateAnimation(delta * speed);

			if (draw->GetCurrentAnimationTime() == 0.0f) {
				ChangeAnimation(L"ReturnWait");
			}
		}
		else {
			draw->UpdateAnimation(delta * moveVec.length() * speed);
		}
	}

	void PlayerAnimationCtrl::ReturnWalk() {
		DefaultPlay(15.0f);
	}

	void PlayerAnimationCtrl::Jump()
	{
		auto draw = GetGameObject()->GetComponent<SmBaseDraw>();

		auto delta = App::GetApp()->GetElapsedTime();
		float speed = 15.0f;


		auto velocity = CalucGravityVelocityY();

		if (velocity >= 0.0f) {  //velocity��0�ȏ�Ȃ�(������Ɉړ���)
			if (draw->GetCurrentAnimationTime() >= 2.0f) { //�Đ���9����ɂȂ�����Đ����~�߂�B
				speed = 0.0f;
			}
		}
		else {  //������
			speed = 0.0f;
		}

		if (!IsJump()) {  //�W�����v��ԂłȂ��Ȃ�
			speed = 30.0f;
		}

		draw->UpdateAnimation(delta * speed);  //�Đ�

		if (draw->GetCurrentAnimationTime() == 0.0f) { //�Đ�����������B
			ChangeAnimation(L"Wait");
		}
		
	}	

	void PlayerAnimationCtrl::Turn() {
		DefaultPlay(20.0f);
	}

	void PlayerAnimationCtrl::HundShake() {
		DefaultPlay();
	}

	void PlayerAnimationCtrl::LookOut() {
		DefaultPlay();
	}

	void PlayerAnimationCtrl::Appeal() {
		DefaultPlay();
	}

	void PlayerAnimationCtrl::Fall() {
		auto delta = App::GetApp()->GetElapsedTime();
		float speed = 15.0f;

		auto draw = GetGameObject()->GetComponent<SmBaseDraw>();

		draw->UpdateAnimation(delta * speed);
	}

	void PlayerAnimationCtrl::Landing() {
		DefaultPlay(30.0f);
	}

	void PlayerAnimationCtrl::Dest() {
		DefaultPlay();
	}

	void PlayerAnimationCtrl::TurnFixed() {
		float speed = 10.0f;
		auto delta = App::GetApp()->GetElapsedTime();

		auto draw = GetGameObject()->GetComponent<SmBaseDraw>();
		draw->UpdateAnimation(delta * speed);
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

	void PlayerAnimationCtrl::ChangeAnimation(const wstring& animeName){//,const bool isCameraLock, const bool lock) {
		auto draw = GetGameObject()->GetComponent<SmBaseDraw>();
		auto nowName = draw->GetCurrentAnimation();
		if (nowName == animeName && nowName != L"Jump") {  //�����A�j���[�V�������A�W�����v�A�j���[�V�����łȂ��Ƃ�
			return;  //�Đ�����A�j���[�V�����������̏ꍇ�X�V���Ȃ��B
		}

		draw->ChangeCurrentAnimation(animeName);

		//�ړ������Ȃ�
		//auto obj = GetGameObject();
		//if (m_isMoveLock) {  //�ړ�����
		//	m_isMoveLock = false;
		//	auto lockComp = obj->GetComponent<Lock>(false);
		//	if (lockComp) {
		//		lockComp->MoveLock(false);
		//	}
		//}

		//if (m_isCameraLook) {  //�J�����̕���������
		//	m_isCameraLook = false;
		//	auto bill = obj->GetComponent<BillBoard>(false);
		//	if (bill) {
		//		bill->SetUpdateActive(false);
		//	}
		//}
	}

	void PlayerAnimationCtrl::ChangeAnimation(const wstring& animeType, const bool isCameraLook, const bool isLock) {
		ChangeAnimation(animeType);

		auto obj = GetGameObject();
		//�ړ�����
		//auto lockComp = obj->GetComponent<Lock>(false);
		//if (lockComp) {
		//	lockComp->MoveLock(isLock);
		//	m_isMoveLock = isLock;
		//}

		//auto bill = obj->GetComponent<BillBoard>(false);
		//if (bill) {
		//	bill->SetUpdateActive(isCameraLook);
		//	m_isCameraLook = isCameraLook;
		//}
	}
}

//endbasecross