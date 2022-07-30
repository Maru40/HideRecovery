
/*!
@file MetaballChildrenPressedManager.cpp
@brief MetaballChildrenPressedManagerのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "MetaballChildrenPressedManager.h"

#include "MetaballRenderBase.h"
#include "PressData.h"
#include "TimeHelper.h"
#include "SoundManager.h"
#include "HitStopManager.h"

#include "WaitTimer.h"
#include "SeekTarget.h"
#include "VelocityManager.h"

#include "MetaballChildrenRender.h"

#include "AutoReflection.h"

namespace basecross {

	namespace Metaball {

		//--------------------------------------------------------------------------------------
		///	パラメータ
		//--------------------------------------------------------------------------------------

		ChildrenPressedManager_Parametor::ChildrenPressedManager_Parametor()
			:ChildrenPressedManager_Parametor(50.0f, 50.0f, 1.0f)
		{}

		ChildrenPressedManager_Parametor::ChildrenPressedManager_Parametor(
			const float& stopCrashPower, 
			const float& slowCrashPower, 
			const float& stopAfterDestroyTime
		) :
			stopCrashPower(stopCrashPower),
			slowCrashPower(slowCrashPower),
			stopAfterDestroyTime(stopAfterDestroyTime)
		{}

		//--------------------------------------------------------------------------------------
		///	潰される管理本体
		//--------------------------------------------------------------------------------------

		ChildrenPressedManager::ChildrenPressedManager(const std::shared_ptr<GameObject>& objPtr):
			ChildrenPressedManager(objPtr, Parametor())
		{}

		ChildrenPressedManager::ChildrenPressedManager(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor) :
			Component(objPtr), m_param(parametor)
		{}

		void ChildrenPressedManager::SlowTimerPress(const PressData& pressData) {
			float pressPower = m_param.slowCrashPower;  //将来的にはメンバから取得
			auto destroyTime = pressData.slowParam.time + m_param.stopAfterDestroyTime;
			auto pressingTime = SlowTimerManager::GetInstance()->GetSlowTimeParametor().time;
			auto pressedTime = destroyTime;
			auto updateTime = pressData.slowParam.time / App::GetApp()->GetStepTimer().GetElapsedTimeSpeed();
			auto deltaType = maru::DeltaType::Normal;

			PlayOneShot(pressData, pressingTime, pressedTime, updateTime, deltaType);
		}

		void ChildrenPressedManager::HitStopPress(const PressData& pressData) {
			float pressPower = m_param.stopCrashPower;
			auto stopData = HitStopManager::GetInstance()->GetStopData();
			auto destroyTime = stopData.startTime + stopData.stopTime + m_param.stopAfterDestroyTime;
			auto pressingTime = stopData.startTime + stopData.stopTime;
			auto pressedTime = stopData.startTime + stopData.stopTime;
			float updateTime = SlowTimerManager::GetInstance()->GetDefaultTimeSpeed();
			auto deltaType = maru::DeltaType::Real;

			PlayOneShot(pressData, pressingTime, pressedTime, updateTime, deltaType);
		}

		void ChildrenPressedManager::PlayOneShot(const PressData& pressData,
			const float& pressingTime, const float& pressedTime,
			const float& updateTime, const maru::DeltaType& deltaType)
		{
			auto waitTimer = GetGameObject()->GetComponent<WaitTimer>(false);
			//潰している時の音
			const float NumLoop = 10.0f;
			const float Volume = 0.5f;
			auto soundItem = SoundManager::GetInstance()->PlayOneShot(SoundManager::ID::SlimePressing, Volume, NumLoop);
			waitTimer->AddTimer(GetThis<Metaball::ChildrenPressedManager>(), pressingTime,
				[&, soundItem]() { soundItem->m_SourceVoice->SetVolume(0); },
				updateTime, deltaType);

			//潰す時に音を出す
			waitTimer->AddTimer(soundItem, pressedTime,
				[&, this]() { SoundManager::GetInstance()->PlayOneShot(SoundManager::ID::SlimePressed); },
				updateTime, deltaType);
		}

		void ChildrenPressedManager::PressCrash(const PressData& pressData) {
			GetGameObject()->AddComponent<WaitTimer>();

			SlowTimerManager::GetInstance()->IsSlow() ? SlowTimerPress(pressData) : HitStopPress(pressData);
		}

		float ChildrenPressedManager::GetPressedTime() const {
			if (SlowTimerManager::GetInstance()->IsSlow()) {
				return SlowTimerManager::GetInstance()->GetSlowTimeParametor().time;
			}
			else {
				auto stopData = HitStopManager::GetInstance()->GetStopData();
				return stopData.startTime + stopData.stopTime;
			}
		}

		float ChildrenPressedManager::GetPressedPower() const {
			if (SlowTimerManager::GetInstance()->IsSlow()) {
				return m_param.slowCrashPower;
			}
			else {
				return m_param.stopCrashPower;
			}
		}

	}
}