/*!
@file SeekTargetOffsetsManager.cpp
@brief SeekTargetOffsetsManagerなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "TaskList.h"
#include "GameTimer.h"
#include "SeekTargetOffsetsManager.h"

#include "SeekTarget.h"
#include "TimeHelper.h"

#include "MetaballRenderBase.h"
#include "MetaballChildrenRender.h"

namespace basecross {

	//パラメータ----------------------------------------------------------------------------------------------------

	SeekTargetOffsetsManager::Parametor::Parametor()
		:Parametor(1.0f, 1.0f, Vec3(0.0f))
	{}

	SeekTargetOffsetsManager::Parametor::Parametor(const float& time, const float& range, const Vec3& offset)
		: time(time), range(range), offset(offset)
	{}

	//--------------------------------------------------------------------------------------------------------------

	//DefaultSeekData-----------------------------------------------------------------------------------------------

	SeekTargetOffsetsManager::DefaultSeekData::DefaultSeekData()
		: DefaultSeekData(Vec3(0.0f), 0.0f)
	{}

	SeekTargetOffsetsManager::DefaultSeekData::DefaultSeekData(const Vec3& offset, const float& range)
		: offset(offset), range(range)
	{}

	//-------------------------------------------------------------------------------------------------------------

	SeekTargetOffsetsManager::SeekTargetOffsetsManager(const std::shared_ptr<GameObject>& owner,
		const vector<ex_weak_ptr<SeekTarget>>& seekTargets
	) :
		m_owner(owner), 
		m_centerMoveParam(Parametor()), 
		m_returnMoveParam(Parametor()),
		m_timer(new GameTimer(0.0f)), 
		m_taskList(new TaskList<TaskEnum>()),
		m_seekTargets(seekTargets)
	{
		DefineTask();
		SetDefaultSeekOffset();
	}

	void SeekTargetOffsetsManager::OnUpdate() {
		if (m_taskList->IsEnd()) {
			return;
		}

		m_taskList->UpdateTask();
	}

	void SeekTargetOffsetsManager::DefineTask() {
		//中心に向かう処理
		m_taskList->DefineTask(TaskEnum::CenterMove,
			[&, this]() { StartCenterMove(); },
			[&, this]() { return UpdateCenterMove(); },
			[]() {}
		);

		//元に戻る処理
		m_taskList->DefineTask(TaskEnum::ReturnMove,
			[&, this]() { StartReturnMove(); },
			[&, this]() { return UpdateReturnMove(); },
			[&, this]() { ExitReturnMove(); });
	}

	void SeekTargetOffsetsManager::SelectTask(const TaskEnum& task) {
		m_taskList->AddTask(task);
	}

	void SeekTargetOffsetsManager::SetDefaultSeekOffset() {
		//オフセットを全て記録する。
		m_defaultSeekData.clear();
		for (auto& seek : m_seekTargets) {
			if (!seek) {
				continue;
			}

			auto offset = seek->GetOffset();
			m_defaultSeekData.push_back(DefaultSeekData(offset, offset.length()));
		}
	}


	Vec3 SeekTargetOffsetsManager::CalculateLerpMovePosition(
		const Vec3& startPosition, const Vec3& endPosition,
		const std::shared_ptr<SeekTarget>& seek, const Parametor& parametor
	)
	{
		const float& time = parametor.time;
		const float& range = parametor.range;
		//時間に合わせて、スピードを計算
		float speed = time == 0 ? range : range / time;
		auto delta = maru::TimeHelper::GetElapsedTime(maru::DeltaType::Normal);

		const float StartTime = 0.0f;
		auto position = Lerp::CalculateLerp(startPosition, endPosition, StartTime, time, speed * delta, Lerp::rate::Linear);

		return position;
	}

	Vec3 SeekTargetOffsetsManager::CalculateTransformMovePosition(const Vec3& startPosition, const Vec3& endPosition,
		const std::shared_ptr<SeekTarget>& seek, const Parametor& parametor)
	{
		const float& time = parametor.time;
		const float& range = parametor.range;
		//時間に合わせて、スピードを計算
		float speed = time == 0 ? range : range / time;
		auto delta = maru::TimeHelper::GetElapsedTime(maru::DeltaType::Normal);

		auto direct = endPosition - startPosition;
		auto position = startPosition + direct.GetNormalized() * speed * delta;

		return position;
	}

	//CenterMove--------------------------------------------------------------------------------------

	void SeekTargetOffsetsManager::StartCenterMove() {
		m_timer->ResetTimer(m_centerMoveParam.time);
		SetDefaultSeekOffset();
	}

	bool SeekTargetOffsetsManager::UpdateCenterMove() {
		for (auto& seek : m_seekTargets) {
			if (!seek) {
				continue;
			}

			auto centerPosition = GetGameObject()->GetComponent<Transform>()->GetPosition();
			auto startPosition = centerPosition + seek->GetOffset();
			auto position = CalculateLerpMovePosition(startPosition, centerPosition, seek.GetShard(), m_centerMoveParam);
			auto endPosition = centerPosition + m_centerMoveParam.offset;

			auto setOffset = position - endPosition;
			seek->SetOffset(setOffset);
		}

		m_timer->UpdateTimer();

		return m_timer->IsTimeUp();
	}

	//-------------------------------------------------------------------------------------------------

	//RetrunMove---------------------------------------------------------------------------------------

	void SeekTargetOffsetsManager::StartReturnMove() {
		m_timer->ResetTimer(m_returnMoveParam.time);
	}

	bool SeekTargetOffsetsManager::UpdateReturnMove() {
		int index = 0;
		for (auto& seek : m_seekTargets) {
			if (!seek) {
				continue;
			}

			auto defaultData = m_defaultSeekData[index++];
			auto centerPosition = GetGameObject()->GetComponent<Transform>()->GetPosition();
			auto startPosition = centerPosition + seek->GetOffset();
			auto endPosition = centerPosition + defaultData.offset;
			auto position = CalculateLerpMovePosition(startPosition, endPosition, seek.GetShard(), m_returnMoveParam);

			auto setOffset = position - centerPosition;
			seek->SetOffset(setOffset);
		}

		m_timer->UpdateTimer();

		return m_timer->IsTimeUp();
	}

	void SeekTargetOffsetsManager::ExitReturnMove() {
		int index = 0;
		for (auto& seek : m_seekTargets) {
			if (!seek) {
				continue;
			}

			auto offset = m_defaultSeekData[index++].offset;
			seek->SetOffset(offset);
		}
	}

	//-------------------------------------------------------------------------------------------------

	//アクセッサ---------------------------------------------------------------------------------------

	void SeekTargetOffsetsManager::StartMove(const TaskEnum& task, const Parametor& parametor) {
		switch (task)
		{
		case TaskEnum::CenterMove:
			m_centerMoveParam = parametor;
			break;

		case TaskEnum::ReturnMove:
			m_returnMoveParam = parametor;
			break;
		}

		SelectTask(task);
	}

	bool SeekTargetOffsetsManager::IsEndTask() {
		return m_taskList->IsEnd();
	}

	std::shared_ptr<GameObject> SeekTargetOffsetsManager::GetGameObject() {
		return m_owner.GetShard();
	}

}

//endbasecross