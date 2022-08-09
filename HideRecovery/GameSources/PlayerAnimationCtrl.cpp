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
			{State::Idle,	L"Idle",       1,    39,   true, 2.0f , &PlayerAnimationCtrl::Idle},
			{State::Walk,	L"Dash",       99,  119,   true, 15.0f, &PlayerAnimationCtrl::Walk},
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
	}

	void PlayerAnimationCtrl::Idle()
	{
		auto delta = App::GetApp()->GetElapsedTime();
		float speed = 15.0f;

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

		const float speed = 2.0f;
		auto delta = App::GetApp()->GetElapsedTime();

		auto moveVec = velocityMananger->GetVelocity();
		
		auto draw = m_drawComponent.lock();
		draw->UpdateAnimation(delta * moveVec.length() * speed);

		//�J�ڏ���
		if (auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false)) {
			if (velocityManager->GetVelocity().length() < m_transitionMember.walkSpeed) {
				ChangeAnimation(State::Idle);
			}
		}
	}

	void PlayerAnimationCtrl::DefaultPlay(const float speed) {
		auto delta = App::GetApp()->GetElapsedTime();
		auto draw = m_drawComponent.lock();

		draw->UpdateAnimation(delta * speed);

		//�J�ڏ���
		if (draw->GetCurrentAnimationTime() == 0.0f) {
			ChangeAnimation(State::Idle);
		}
	}

	void PlayerAnimationCtrl::ChangeAnimation(const State& state) {
		if (m_currentState == state) {
			return;
		}

		m_currentState = state;
		m_drawComponent.lock()->ChangeCurrentAnimation(m_parametorMap[state].stateName);
	}

	void PlayerAnimationCtrl::ChangeForceAnimation(const State& state) {
		m_currentState = state;
		m_drawComponent.lock()->ChangeCurrentAnimation(m_parametorMap[state].stateName);
	}

}

//endbasecross