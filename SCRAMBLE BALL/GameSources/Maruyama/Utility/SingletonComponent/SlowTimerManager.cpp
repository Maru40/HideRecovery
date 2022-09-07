/*!
@file SlowTimerManager.cpp
@brief SlowTimerManager�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "SlowTimerManager.h"
#include "Maruyama/Utility/Timer/GameTimer.h"

#include "Maruyama/Utility/Timer/TimeHelper.h"

namespace basecross {

	std::weak_ptr<SlowTimerManager> maru::SingletonComponent<SlowTimerManager>::sm_instance;

	//--------------------------------------------------------------------------------------
	/// �X���[�^�C�}�[�p�����[�^
	//--------------------------------------------------------------------------------------

	SlowTimeParametor::SlowTimeParametor()
		:SlowTimeParametor(1.0f, 0.5f)
	{}

	SlowTimeParametor::SlowTimeParametor(const float& time, const float& timeSpeed)
		:time(time), timeSpeed(timeSpeed)
	{}

	//--------------------------------------------------------------------------------------
	/// �X���[�^�C�}�[�Ǘ��p�����[�^
	//--------------------------------------------------------------------------------------

	SlowTimerManager::Parametor::Parametor()
		: Parametor(1.0f, SlowTimeParametor())
	{}
	SlowTimerManager::Parametor::Parametor(const float& defaultTimeSpeed, const SlowTimeParametor& timeParam)
		: Parametor(defaultTimeSpeed, timeParam, SpeedType::Lerp)
	{}
	SlowTimerManager::Parametor::Parametor(const float& defaultTimeSpeed, const SlowTimeParametor& timeParam, const SpeedType& speedType)
		: defaultTimeSpeed(defaultTimeSpeed), timeParam(timeParam), speedType(speedType)
	{}

	//--------------------------------------------------------------------------------------
	/// �X���[�^�C�}�[�Ǘ��N���X�{��
	//--------------------------------------------------------------------------------------

	SlowTimerManager::SlowTimerManager(const std::shared_ptr<GameObject>& objPtr)
		:maru::SingletonComponent<SlowTimerManager>(objPtr), m_timer(new GameTimer(0.0f))
	{}

	void SlowTimerManager::OnUpdate() {
		if (m_timer->IsTimeUp()) {
			return;
		}

		CalculateUpdateTimeSpeed();
		m_timer->UpdateTimer(m_param.defaultTimeSpeed, maru::DeltaType::Real);

		if (m_timer->IsTimeUp()) {
			EndTime();
		}
	}

	void SlowTimerManager::OnDestroy() {
		ForceStop();

		SingletonComponent::OnDestroy();
	}

	float SlowTimerManager::CalculateUpdateTimeSpeed() {
		switch (m_param.speedType)
		{
		case Parametor::SpeedType::Normal:
			return CalculateNormalTimerSpeed();
		case Parametor::SpeedType::Lerp:
			return CalculateLerpTimerSpeed();
		}

		return CalculateNormalTimerSpeed();
	}

	float SlowTimerManager::CalculateNormalTimerSpeed() {
		auto& stepTimer = App::GetApp()->GetStepTimer();
		return m_param.defaultTimeSpeed / m_param.timeParam.timeSpeed;
	}

	float SlowTimerManager::CalculateLerpTimerSpeed() {
		auto& stepTimer = App::GetApp()->GetStepTimer();

		auto speed = stepTimer.GetElapsedTimeSpeed();
		float StartTime = 0.0f;
		float EndTime = 1.0f;
		auto delta = maru::TimeHelper::GetElapsedTime(maru::DeltaType::Real);
		auto newSpeed = Lerp::CalculateLerp(speed, m_param.defaultTimeSpeed, StartTime, EndTime, m_timer->GetTimeRate(), Lerp::rate::Linear);
		stepTimer.SetElapsedTimeSpeed(newSpeed);
		return m_param.defaultTimeSpeed / m_param.timeParam.timeSpeed;
	}

	void SlowTimerManager::EndTime() {
		auto& stepTimer = App::GetApp()->GetStepTimer();
		stepTimer.SetElapsedTimeSpeed(m_param.defaultTimeSpeed);
	}

	void SlowTimerManager::StartTimer(const float& time, const float& slowTimeSpeed) {
		if (IsSlow()) {
			return;
		}

		m_timer->ResetTimer(time);
		auto& stepTimer = App::GetApp()->GetStepTimer();
		//�p�����[�^�̕ۑ�
		m_param.defaultTimeSpeed = stepTimer.GetElapsedTimeSpeed();
		m_param.timeParam.time = time;
		m_param.timeParam.timeSpeed = slowTimeSpeed;

		stepTimer.SetElapsedTimeSpeed(slowTimeSpeed);
	}

	void SlowTimerManager::ForceStop() {
		m_timer->ResetTimer(0.0f);
		EndTime();
	}

	bool SlowTimerManager::IsSlow() const{
		return !m_timer->IsTimeUp();
	}

	SlowTimeParametor SlowTimerManager::GetSlowTimeParametor() const {
		return m_param.timeParam;
	}
}

//endbasecross