/*!
@file StartCamera.cpp
@brief StartCamera�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "StartCamera.h"
#include "Maruyama/Utility/Mathf.h"

#include "GameStageBase.h"

#include "Maruyama/Utility/Utility.h"
#include "Patch/PlayerInputer.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�J�����̈ړ��Ǘ��p�����[�^
	//--------------------------------------------------------------------------------------

	CameraMoveParametor::CameraMoveParametor() :
		startPosition(Vec3(0.0f)),
		endPosition(Vec3(0.0f)), 
		speed(0.0f)
	{}

	CameraMoveParametor::CameraMoveParametor(
		const Vec3& startPosition,
		const float& speed
	) :
		startPosition(startPosition),
		endPosition(startPosition),
		speed(speed)
	{}

	CameraMoveParametor::CameraMoveParametor(
		const Vec3& startPosition,
		const Vec3& endPosition,
		const float& speed
	) :
		startPosition(startPosition),
		endPosition(endPosition),
		speed(speed)
	{}

	//--------------------------------------------------------------------------------------
	///	�J�n�J�������o�p�̃p�����[�^
	//--------------------------------------------------------------------------------------

	StartCamera::ShowParametor::ShowParametor() :
		ShowParametor(CameraMoveParametor(),
			CameraMoveParametor(),
			3.0f)
	{}

	StartCamera::ShowParametor::ShowParametor(const CameraMoveParametor& eye,
		const CameraMoveParametor& at,
		const float lookTime
	) :
		eye(eye),
		at(at),
		lookTime(lookTime)
	{}

	//--------------------------------------------------------------------------------------
	///	�J�n���K�p�̃J����
	//--------------------------------------------------------------------------------------

	void StartCamera::OnCreate()
	{
		if (m_params.size() == 0) {
			ChangeCamera();
			return;
		}

		SetAt(m_params[m_index].at.startPosition);
		SetEye(m_params[m_index].eye.startPosition);
	}

	void StartCamera::OnUpdate()
	{
		if (m_params.size() == 0) {
			ChangeCamera();
			return;
		}

		//�I���̂��m�点
		if (m_params.size() <= m_index) {
			return; //�I��
		}

		Skip();
		
		auto delta = App::GetApp()->GetElapsedTime();

		EyeMove();
		AtMove();
		
		m_timeElapsed += delta;
		if (m_timeElapsed >= m_params[m_index].lookTime)
		{
			m_timeElapsed = 0.0f;
			m_index++;
			if (IsEnd()) {  //�I����������
				ChangeCamera();
			}
			else {
				SetEye(m_params[m_index].eye.startPosition);
				SetAt(m_params[m_index].at.startPosition);
			}
		}

	}

	std::shared_ptr<GameStageBase> StartCamera::GetGameStage() {
		//����nullptr�ŋ����I����
		return nullptr;

		auto& app = App::GetApp();           // �A�v���P�[�V�����I�u�W�F�N�g���擾
		auto scene = app->GetScene<Scene>(); // �A�v���I�u�W�F����V�[�����擾
		auto stage = dynamic_pointer_cast<GameStageBase>(scene->GetActiveStage());

		if (stage) {
			return stage;
		}
		else {
			return nullptr;
		}
	}

	void StartCamera::EyeMove()
	{
		auto delta = App::GetApp()->GetElapsedTime();

		auto pos = GetEye();
		const auto& endPosition = m_params[m_index].eye.endPosition;
		const float speed = m_params[m_index].eye.speed;

		auto newPos = Lerp::CalculateLerp(pos, endPosition, 0.0f, 1.0f, delta * speed, Lerp::rate::Linear);

		SetEye(newPos);
	}


	void StartCamera::AtMove()
	{
		auto delta = App::GetApp()->GetElapsedTime();

		auto at = GetAt();
		const auto& endAt = m_params[m_index].at.endPosition;
		const float speed = m_params[m_index].at.speed;

		auto newAt = Lerp::CalculateLerp(at, endAt, 0.0f, 1.0f, delta * speed, Lerp::rate::Linear);

		SetAt(newAt);
	}

	void StartCamera::Skip(){
		if (PlayerInputer::IsDecision()) {
			ChangeCamera();
		}
	}

	void StartCamera::ChangeCamera() {
		//�I��,MainCamera�ɐ؂�ւ�
		auto& app = App::GetApp(); // �A�v���P�[�V�����I�u�W�F�N�g���擾
		auto scene = app->GetScene<Scene>(); // �A�v���I�u�W�F����V�[�����擾
		auto stage = GetGameStage();

		if (stage) {
			stage->ChangeMainCamera();
		}
	}

}

//endbasecross