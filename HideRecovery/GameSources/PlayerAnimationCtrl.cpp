/*!
@file PlayerAnimationCtrl.cpp
@brief PlayerAnimationCtrlクラス実体
担当：丸山裕喜
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
		//コンポーネントが存在するかの確認
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
		//	return false;  //0.0f方向に重力がかかっいない == ジャンプ中でない
		//}
		//else {
		//	return true;  //ジャンプ中
		//}
		return false;
	}


	void PlayerAnimationCtrl::OnCreate() 
	{
		auto obj = GetGameObject();

		//メッシュの調整用Mat
		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(1.0f),
			Vec3(0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, -1.0f, 0.0f)
		);

		//メッシュの生成
		auto drawComp = obj->GetComponent<PNTBoneModelDraw>();
		//drawComp->SetMeshResource(L"Player_Mesh");
		//drawComp->SetMeshToTransformMatrix(spanMat);
		//auto shadow = obj->AddComponent<Shadowmap>();
		//shadow->SetMeshResource(L"Player_Mesh");

		//アニメーション用のパラメータ
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
			//{L"Landing",   310,  320,  true, &PlayerAnimationCtrl::Landing},  //着地
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

		ChangeAnimation(params[0].name);  //初期アニメ―ション設定
	}


	void PlayerAnimationCtrl::OnUpdate()
	{
		auto draw = GetGameObject()->GetComponent<SmBaseDraw>();
		auto name = draw->GetCurrentAnimation();

		m_animations[name](*(this));

		//簡易遷移文
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
		//アニメーション確認用
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

		//タイムが0で無い、かつ、moveVecが0.0fだったら。
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

		if (velocity >= 0.0f) {  //velocityが0以上なら(上方向に移動中)
			if (draw->GetCurrentAnimationTime() >= 2.0f) { //再生が9より上になったら再生を止める。
				speed = 0.0f;
			}
		}
		else {  //落下中
			speed = 0.0f;
		}

		if (!IsJump()) {  //ジャンプ状態でないなら
			speed = 30.0f;
		}

		draw->UpdateAnimation(delta * speed);  //再生

		if (draw->GetCurrentAnimationTime() == 0.0f) { //再生完了したら。
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
		if (nowName == animeName && nowName != L"Jump") {  //同じアニメーション且つ、ジャンプアニメーションでないとき
			return;  //再生するアニメーションが同じの場合更新しない。
		}

		draw->ChangeCurrentAnimation(animeName);

		//移動制限など
		//auto obj = GetGameObject();
		//if (m_isMoveLock) {  //移動制限
		//	m_isMoveLock = false;
		//	auto lockComp = obj->GetComponent<Lock>(false);
		//	if (lockComp) {
		//		lockComp->MoveLock(false);
		//	}
		//}

		//if (m_isCameraLook) {  //カメラの方向を向く
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
		//移動制限
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