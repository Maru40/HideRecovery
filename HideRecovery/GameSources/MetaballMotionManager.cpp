
/*!
@file MetaballMotionManager.cpp
@brief MetaballMotionManagerのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "TaskList.h"
#include "SeekTarget.h"
#include "MetaballChildrenRender.h"
#include "MetaballMotionManager.h"

#include "MetaballRenderBase.h"

#include "TimeHelper.h"
#include "GameTimer.h"

#include "SlimeCore.h"

#include "MaruUtility.h"
#include "MaruAction.h"

namespace basecross {

	namespace Metaball {

		namespace Task {

			//--------------------------------------------------------------------------------------
			///	パラメータ
			//--------------------------------------------------------------------------------------

			SeekOffsetMove_Parametor::SeekOffsetMove_Parametor()
				:SeekOffsetMove_Parametor(1.0f, 0.5f)
			{}

			SeekOffsetMove_Parametor::SeekOffsetMove_Parametor(const float& time, const float& radius)
				:time(time), radius(radius), coreSeekParam(SeekTarget::Parametor(10.0f, 0.0f, Vec3(0.0f), SeekTarget::SeekType::Lerp)), firstSpeed(1.0f)
			{}

			//--------------------------------------------------------------------------------------
			///	追従オフセット移動本体
			//--------------------------------------------------------------------------------------

			SeekOffsetMove::SeekOffsetMove(const std::shared_ptr<GameObject>& owner, const std::shared_ptr<Parametor>& paramPtr)
				:TaskNodeBase(owner), m_paramPtr(paramPtr), m_timer(new GameTimer(0.0f))
			{
				SettingCore();
				SettingSeekTargets();
			}

			void SeekOffsetMove::OnStart() {
				if (m_seekTargets.size() == 0) {
					SettingSeekTargets();
				}

				SettingSeekTargets();

				SaveLerpDatas();
				m_timer->ResetTimer(m_paramPtr->time);
			}

			bool SeekOffsetMove::OnUpdate() {
				//auto speed = 1.0f / m_paramPtr->time;
				float speed = CalculateUpdateTimeSpeed();
				m_timer->UpdateTimer(speed);

				int index = 0;
				for (const auto& seek : m_seekTargets) {
					//seekがない、又は、パラメータのサイズを超えた場合
					if (!seek || m_paramPtr->seekParams.size() <= index || m_paramPtr->seekParams.size() <= index) {
						index++;
						continue;
					}

					const auto& saveLerpData = m_saveLerpDatas[index];
					const auto& seekParam = m_paramPtr->seekParams[index];
					SeekOffsetUpdate(seek.GetShard(), seekParam, saveLerpData);

					auto render = seek->GetGameObject()->GetComponent<Metaball::ChildrenRender>(false);
					RadiusUpdate(render, m_paramPtr->radius, speed, saveLerpData.radius);
					index++;
				}

				//コアの移動処理
				if (m_core) {
					auto seek = m_core->GetSeekTargetPtr();
					if (seek) {
						SeekOffsetUpdate(seek, m_paramPtr->coreSeekParam, m_coreSaveLerpData);
					}
				}

				return m_timer->IsTimeUp();
			}

			void SeekOffsetMove::OnExit() {

				return;
				auto index = 0;
				for (auto seek : m_seekTargets) {
					if (!seek || m_paramPtr->seekParams.size() < index) {
						index++;
						continue;
					}

					auto seekParam = m_paramPtr->seekParams[index++];
					seek->SetOffset(seekParam.offset);

					auto render = seek->GetGameObject()->GetComponent<Metaball::ChildrenRender>(false);
					if (render) {
						render->SetRadius(m_paramPtr->radius);
					}
				}
			}

			void SeekOffsetMove::SettingCore() {
				m_core = GetOwner()->GetComponent<Enemy::SlimeCore>(false);
			}

			void SeekOffsetMove::SettingSeekTargets() {
				auto render = GetOwner()->GetComponent<Metaball::RenderBase>(false);
				if (render) {
					m_seekTargets = render->GetChildrenComponents<SeekTarget>();
				}
			}

			void SeekOffsetMove::SeekOffsetUpdate(const std::shared_ptr<SeekTarget>& seek,
				const SeekTarget::Parametor& seekParam,
				const SaveLerpData& saveLerpData)
			{
				SeekOffsetLerpUpdate(seek, seekParam, saveLerpData);
			}

			void SeekOffsetMove::SeekOffsetLerpUpdate(const std::shared_ptr<SeekTarget>& seek,
				const SeekTarget::Parametor& seekParam,
				const SaveLerpData& saveLerpData)
			{
				auto centerPosition = GetOwner()->GetComponent<Transform>()->GetPosition();
				auto startPosition = saveLerpData.startOffset;
				auto endPosition = seekParam.offset;
				auto offset = CalcuLerpMovePosition(startPosition, endPosition);

				auto setSeekParam = seekParam;
				setSeekParam.offset = offset;
				seek->SetParametor(setSeekParam);
			}

			void SeekOffsetMove::RadiusUpdate(const std::shared_ptr<Metaball::ChildrenRender>& render,
				const float& radius, const float& speed, const float& startRadius)
			{
				auto delta = maru::TimeHelper::GetElapsedTime(maru::DeltaType::Normal);
				const float StartTime = 0.0f;
				const float EndTime = 1.0f;
				float newRadius = Lerp::CalculateLerp(startRadius, radius, StartTime, EndTime, m_timer->GetTimeRate(), Lerp::rate::Linear);
				render->SetRadius(newRadius);
			}

			Vec3 SeekOffsetMove::CalcuLerpMovePosition(const Vec3& startPosition, const Vec3& endPosition)
			{
				const float StartTime = 0.0f;
				const float EndTime = 1.0f;
				
				auto position = Lerp::CalculateLerp(startPosition, endPosition, StartTime, EndTime, m_timer->GetTimeRate(), Lerp::rate::Linear);

				return position;
			}

			float SeekOffsetMove::CalculateUpdateTimeSpeed() {
				float speed = 1.0f;
				if (m_paramPtr->firstSpeed > 1.0f) {
					speed = m_paramPtr->firstSpeed * m_timer->GetIntervalTimeRate();
				}
				if (m_paramPtr->firstSpeed < 1.0f) {
					auto base = 1.0f / m_paramPtr->firstSpeed;
					auto rate = m_timer->GetIntervalTimeRate();
					auto temp = base * m_timer->GetIntervalTimeRate();
					speed = 1.0f / (base * m_timer->GetIntervalTimeRate());
				}

				return speed;
			}

			void SeekOffsetMove::SaveLerpDatas() {
				maru::Action<void()> removeAction;
				m_saveLerpDatas.clear();
				for (const auto& seek : m_seekTargets) {
					if (!seek) {
						removeAction.AddFunction([&, seek]() { maru::Utility::RemoveVec(m_seekTargets, seek); });  //nullptrは削除
						continue;
					}

					auto render = seek->GetGameObject()->GetComponent<Metaball::ChildrenRender>(false);
					auto offset = seek->GetLocalOffset();
					if (render) {
						m_saveLerpDatas.push_back(SaveLerpData(render->GetLocalRadius(), offset));
					}	
				}

				//コア情報を取得
				if (m_core) {
					m_coreSaveLerpData.startOffset = m_core->GetSeekParametor().offset;
					m_coreSaveLerpData.velocity = Vec3(0.0f);
				}

				removeAction.Invoke();
			}

			bool SeekOffsetMove::IsEnd() {
				return true;
			}

		}

		//--------------------------------------------------------------------------------------
		///	モーション管理クラス
		//--------------------------------------------------------------------------------------

		MotionManager::MotionManager(const std::shared_ptr<GameObject>& owner)
			:m_owner(owner), m_taskList(new TaskList<TaskEnum>()), m_currentSeekOffetParametor(new Task::SeekOffsetMove_Parametor())
		{
			DefineTask();
		}

		void MotionManager::OnUpdate() {
			m_taskList->UpdateTask();

			if (m_taskList->IsEnd()) {
				OneTaskEndFunction();
			}
		}

		void MotionManager::DefineTask() {
			m_taskList->DefineTask(TaskEnum::Move, std::make_shared<Task::SeekOffsetMove>(GetOwner(), m_currentSeekOffetParametor));
		}

		void MotionManager::SelectTask(const TaskEnum& task) {
			m_taskList->AddTask(task);
		}

		void MotionManager::OneTaskEndFunction() {
			if (ChangeCurrentParametor()) {
				SelectTask(TaskEnum::Move);
			}
		}

		bool MotionManager::ChangeCurrentParametor() {
			if (m_seekOffsetParametors.size() != 0) {
				const auto& parametor = m_seekOffsetParametors.front(); //先頭要素取得
				*m_currentSeekOffetParametor.get() = parametor;
				m_seekOffsetParametors.pop(); //先頭要素削除
				return true;
			}

			return false;
		}

		//--------------------------------------------------------------------------------------
		///	モーション管理のアクセッサ
		//--------------------------------------------------------------------------------------

		void MotionManager::AddTask(const TaskEnum& taskEnum, const Task::SeekOffsetMove_Parametor& parametor) {
			m_seekOffsetParametors.push(parametor);
			if (m_taskList->IsEnd()) {
				ChangeCurrentParametor();
			}
			SelectTask(taskEnum);
		}

		void MotionManager::ForceStop(const bool isExitFunction) {
			m_taskList->ForceStop(isExitFunction);
		}

		bool MotionManager::IsEnd() {
			return m_taskList->IsEnd() && m_seekOffsetParametors.size() == 0;
		}

	}

}