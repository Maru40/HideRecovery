/*!
@file PlayerAnimationCtrl.cpp
@brief PlayerAnimationCtrl�N���X����
�S���F�ێR�T��
*/


#include "stdafx.h"
#include "Project.h"

#include "PlayerAnimationCtrl.h"

#include "VelocityManager.h"
#include "PlayerInputer.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �J�ڃ����o�[
	//--------------------------------------------------------------------------------------

	PlayerAnimationCtrl_TransitionMember::PlayerAnimationCtrl_TransitionMember():
		walkSpeed(2.0f)
	{}

	//--------------------------------------------------------------------------------------
	/// �A�j���[�V�����Ǘ��{��
	//--------------------------------------------------------------------------------------

	PlayerAnimationCtrl::PlayerAnimationCtrl(const std::shared_ptr<GameObject>& objPtr):
		AnimationCtrl(objPtr), m_currentState(State(0)), m_transitionMember(TransitionMember())
	{}

	void PlayerAnimationCtrl::OnCreate() 
	{
		auto object = GetGameObject();

		//���b�V���̐���
		m_drawComponent = GetGameObject()->GetComponent<SmBaseDraw>();
		auto drawComp = object->GetComponent<PNTBoneModelDraw>();

		//�A�j���[�V�����p�̃p�����[�^
		AnimationParametor<PlayerAnimationCtrl> params[] = {
			//�X�e�[�gEnum,				�X�e�[�g��,				�J�n����,	�I������,	���[�v,		�X�V���x,	�X�V�C�x���g
			{State::Idle,				L"Idle",				1,			39,			true,		1.0f ,		&PlayerAnimationCtrl::Idle},
			{State::Walk,				L"Dash",				99,			119,		true,		0.1f,		&PlayerAnimationCtrl::Walk},
			{State::PutItem_Floor,		L"PutItem_Floor",		125,		149,		true,		1.0f,		&PlayerAnimationCtrl::PutItem_Floor},
			{State::PutItem_HideObject,	L"PutItem_HideObject",	150,		174,		true,		1.0f,		&PlayerAnimationCtrl::PutItem_HideObject},
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

			m_parametorMap[param.stateType] = param;
		}

		ChangeForceAnimation(params[0].stateType);  //�����A�j���\�V�����ݒ�
	}

	void PlayerAnimationCtrl::OnStart() {
		m_drawComponent = GetGameObject()->GetComponent<SmBaseDraw>();
	}

	void PlayerAnimationCtrl::OnUpdate()
	{
		m_parametorMap[m_currentState].updateEvent(*(this));

		//�f�o�b�O--------------------------------------------------
		//if (PlayerInputer::IsBDown()) {
		//	ChangeAnimation(State::PutItem_Floor);
		//}
		//if (PlayerInputer::IsYDown()) {
		//	ChangeAnimation(State::PutItem_HideObject);
		//}
	}

	//--------------------------------------------------------------------------------------
	/// �A�j���[�V�����C�x���g
	//--------------------------------------------------------------------------------------

	void PlayerAnimationCtrl::Idle()
	{
		auto delta = App::GetApp()->GetElapsedTime();
		float speed = GetCurrentUpdateSpeed();

		m_drawComponent.lock()->UpdateAnimation(delta * speed);

		//�J�ڏ���
		if (auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false)) {
			if (m_transitionMember.walkSpeed < velocityManager->GetVelocity().length()) {
				ChangeAnimation(State::Walk);
			}
		}
	}

	void PlayerAnimationCtrl::Walk()
	{
		auto velocityMananger = GetGameObject()->GetComponent<VelocityManager>(false);

		float delta = App::GetApp()->GetElapsedTime();
		auto moveVec = velocityMananger->GetVelocity();
		
		m_drawComponent.lock()->UpdateAnimation(delta * moveVec.length() * GetCurrentUpdateSpeed());

		//�J�ڏ���
		if (auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false)) {
			if (velocityManager->GetVelocity().length() < m_transitionMember.walkSpeed) {
				ChangeAnimation(State::Idle);
			}
		}
	}

	void PlayerAnimationCtrl::PutItem_Floor(){
		DefaultPlay();
	}

	void PlayerAnimationCtrl::PutItem_HideObject() {
		DefaultPlay();
	}

	void PlayerAnimationCtrl::DefaultPlay(const bool isEndTransitionIdle) {
		auto delta = App::GetApp()->GetElapsedTime();
		auto draw = m_drawComponent.lock();

		draw->UpdateAnimation(delta * GetCurrentUpdateSpeed());

		//�J�ڏ���
		if (isEndTransitionIdle && draw->GetCurrentAnimationTime() == 0.0f) {
			ChangeAnimation(State::Idle);
		}
	}

	//--------------------------------------------------------------------------------------
	/// �A�N�Z�b�T
	//--------------------------------------------------------------------------------------

	void PlayerAnimationCtrl::ChangeAnimation(const State& state) {
		if (m_currentState == state) {
			return;
		}

		ChangeForceAnimation(state);
	}

	void PlayerAnimationCtrl::ChangeForceAnimation(const State& state) {
		//�I���C�x���g�Đ�
		if (auto& exitEvent = m_parametorMap[m_currentState].exitEvent) {
			exitEvent(*this);
		}

		m_currentState = state;
		m_drawComponent.lock()->ChangeCurrentAnimation(m_parametorMap[state].stateName);

		//�X�^�[�g�C�x���g�Đ�
		if (auto& startEvent = m_parametorMap[m_currentState].startEvent) {
			startEvent(*this);
		}
	}

}

//endbasecross