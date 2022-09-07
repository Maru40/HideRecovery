/*!
@file HitStopManager.cpp
@brief HitStopManager����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Utility/Timer/TimeHelper.h"
#include "Maruyama/Utility/Timer/GameTimer.h"
#include "HitStopManager.h"

#include "Patch/PlayerInputer.h"

namespace basecross {

	std::weak_ptr<HitStopManager> maru::SingletonComponent<HitStopManager>::sm_instance;

	//--------------------------------------------------------------------------------------
	///	�q�b�g�X�g�b�v�f�[�^
	//--------------------------------------------------------------------------------------

	HitStopManager::Data::Data()
		:Data(0.0f, 0.0f)
	{}

	HitStopManager::Data::Data(const float& stopTime, const float& startTime)
		:stopTime(stopTime), startTime(startTime)
	{}

	//--------------------------------------------------------------------------------------
	///	�p�����[�^
	//--------------------------------------------------------------------------------------

	HitStopManager::Parametor::Parametor()
		:Parametor(Data(), 0.0f, nullptr)
	{}

	HitStopManager::Parametor::Parametor(const Data& hitStopData, const float& defaultSpeed, const std::function<void()>& endFunction)
		:hitStopData(hitStopData), defaultSpeed(defaultSpeed), endFunction(endFunction)
	{}

	//--------------------------------------------------------------------------------------
	///	�q�b�g�X�g�b�v�Ǘ��{��
	//--------------------------------------------------------------------------------------

	HitStopManager::HitStopManager(const std::shared_ptr<GameObject>& objPtr)
		:SingletonComponent(objPtr), m_param(Parametor()), m_timer(new GameTimer(0.0f))
	{}

	void HitStopManager::OnUpdate() {
		const float UpdateTime = 1.0f;
		m_timer->UpdateTimer(UpdateTime, maru::DeltaType::Real);
	}

	void HitStopManager::StartStop(const float& stopTime) {
		auto& stepTimer = App::GetApp()->GetStepTimer();
		m_param.defaultSpeed = stepTimer.GetElapsedTimeSpeed();
		stepTimer.SetElapsedTimeSpeed(0.0f);
		//stopTime��ɃX�g�b�v����
		m_timer->ResetTimer(stopTime, [&, this]() { EndTimer(); });
	}

	void HitStopManager::EndTimer() {
		auto& stepTimer = App::GetApp()->GetStepTimer();
		stepTimer.SetElapsedTimeSpeed(m_param.defaultSpeed);
		if (m_param.endFunction) {
			m_param.endFunction();
		}
	}

	//--------------------------------------------------------------------------------------
	///	�A�N�Z�b�T
	//--------------------------------------------------------------------------------------

	void HitStopManager::StartTimer(const float& stopTime, const float& startTime, const std::function<void()>& endFunction) {
		if (IsHitStop()) {
			return;
		}

		auto& stepTimer = App::GetApp()->GetStepTimer();
		m_param.hitStopData.startTime = startTime;
		m_param.hitStopData.stopTime = stopTime;
		m_param.defaultSpeed = stepTimer.GetElapsedTimeSpeed();
		m_param.endFunction = endFunction;
		//startTime��ɏ���������
		m_timer->ResetTimer(startTime, [&, this, stopTime]() { StartStop(stopTime); });
	}

	HitStopManager::Data HitStopManager::GetStopData() const noexcept {
		return m_param.hitStopData;
	}

	bool HitStopManager::IsHitStop() const{
		return !m_timer->IsTimeUp(); //�^�C�}�[�������Ă�����ׂ��Ă���r��
	}

}

//endbasecross