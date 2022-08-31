/*!
@file HitStopManager.cpp
@brief HitStopManager実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "TimeHelper.h"
#include "GameTimer.h"
#include "HitStopManager.h"

#include "PlayerInputer.h"
#include "DebugObject.h"

namespace basecross {

	std::weak_ptr<HitStopManager> maru::SingletonComponent<HitStopManager>::sm_instance;

	//--------------------------------------------------------------------------------------
	///	ヒットストップデータ
	//--------------------------------------------------------------------------------------

	HitStopManager::Data::Data()
		:Data(0.0f, 0.0f)
	{}

	HitStopManager::Data::Data(const float& stopTime, const float& startTime)
		:stopTime(stopTime), startTime(startTime)
	{}

	//--------------------------------------------------------------------------------------
	///	パラメータ
	//--------------------------------------------------------------------------------------

	HitStopManager::Parametor::Parametor()
		:Parametor(Data(), 0.0f, nullptr)
	{}

	HitStopManager::Parametor::Parametor(const Data& hitStopData, const float& defaultSpeed, const std::function<void()>& endFunction)
		:hitStopData(hitStopData), defaultSpeed(defaultSpeed), endFunction(endFunction)
	{}

	//--------------------------------------------------------------------------------------
	///	ヒットストップ管理本体
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
		//stopTime後にストップ解除
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
	///	アクセッサ
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
		//startTime後に処理を実装
		m_timer->ResetTimer(startTime, [&, this, stopTime]() { StartStop(stopTime); });
	}

	HitStopManager::Data HitStopManager::GetStopData() const noexcept {
		return m_param.hitStopData;
	}

	bool HitStopManager::IsHitStop() const{
		return !m_timer->IsTimeUp(); //タイマーが動いていたら潰している途中
	}

}

//endbasecross