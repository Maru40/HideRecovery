/*!
@file Task_CameraAtMove.cpp
@brief PanoramicViewingのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Task_CameraAtMove.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	パラメータ
	//--------------------------------------------------------------------------------------

	Task_CameraAtMove::Parametor::Parametor() 
		:Parametor(Vec3(0.0f), Vec3(0.0f), 1.0f, MoveType::Lerp)
	{}
	Task_CameraAtMove::Parametor::Parametor(
		const Vec3& startPosition,
		const Vec3& endPosition,
		const float& speed,
		const MoveType moveType,
		const DeltaType& deltaType
	) :
		startPosition(startPosition),
		endPosition(endPosition),
		speed(speed),
		moveType(moveType),
		deltaType(deltaType)
	{}

	//--------------------------------------------------------------------------------------
	///	パン移動タスク本体
	//--------------------------------------------------------------------------------------

	Task_CameraAtMove::Task_CameraAtMove(const std::shared_ptr<Camera> camera)
		:Task_CameraAtMove(camera, Parametor())
	{}

	Task_CameraAtMove::Task_CameraAtMove(const std::shared_ptr<Camera> camera, const Parametor& parametor)
		:Task_CameraAtMove(camera, vector<Parametor>(1, parametor))
	{}

	Task_CameraAtMove::Task_CameraAtMove(const std::shared_ptr<Camera> camera, const vector<Parametor>& parametors)
		:TaskNodeBase<Camera>(camera), m_params(parametors)
	{}

	void Task_CameraAtMove::OnStart() {
		m_currentIndex = 0;

		if (IsEnd()) { //パラメータが0なら処理をしない
			return;
		}
		GetOwner()->SetAt(m_params[0].startPosition);
	}

	bool Task_CameraAtMove::OnUpdate() {
		if (IsEnd()) {
			return IsEnd();
		}

		MoveUpdate();

		if (IsNext(m_params[m_currentIndex])) {
			m_currentIndex++;
		}

		return IsEnd();
	}

	void Task_CameraAtMove::OnExit() {

	}

	void Task_CameraAtMove::MoveUpdate() {
		auto param = m_params[m_currentIndex];

		switch (param.moveType)
		{
		case MoveType::Lerp:
			LerpMove(param);
			break;

		case MoveType::Transform:
			TransformMove(param);
			break;
		}
	}

	void Task_CameraAtMove::LerpMove(const Parametor& param) {
		auto owner = GetOwner();
		auto delta = GetElapsedTime(param);
		auto speed = param.speed * delta;

		const float StartTime = 0.0f;
		const float EndTime = 1.0f;
	 	auto position = Lerp::CalculateLerp(owner->GetAt(), param.endPosition, StartTime, EndTime, speed, Lerp::rate::Linear);

		owner->SetAt(position);
	}

	void Task_CameraAtMove::TransformMove(const Parametor& param) {
		auto owner = GetOwner();
		auto delta = GetElapsedTime(param);
		auto speed = param.speed * delta;
		auto direction = param.GetToEndDirection();

		auto position = owner->GetAt();
		position += direction * speed;

		owner->SetAt(position);
	}

	bool Task_CameraAtMove::IsNext(const Parametor& param) {
		auto toEndPosition = param.endPosition - GetOwner()->GetAt();

		const float BetweenLength = 0.01f;
		return toEndPosition.length() < BetweenLength ? true : false;
	}

	float Task_CameraAtMove::GetElapsedTime(const Parametor& param) {
		auto& stepTimer = App::GetApp()->GetStepTimer();

		switch (param.deltaType) 
		{
		case DeltaType::Normal:
			return static_cast<float>(stepTimer.GetElapsedSeconds());
			break;

		case DeltaType::Real:
			return static_cast<float>(stepTimer.GetDefaultElapsedSeconds());
			break;

		default:
			return static_cast<float>(stepTimer.GetElapsedSeconds());
			break;
		};
	}

	//--------------------------------------------------------------------------------------
	///	アクセッサ
	//--------------------------------------------------------------------------------------

	bool Task_CameraAtMove::IsEnd() {
		return m_currentIndex >= m_params.size() ? true : false;
	}

	void Task_CameraAtMove::AddParametor(const Parametor& parametor) {
		m_params.push_back(parametor);
	}

	Task_CameraAtMove::Parametor Task_CameraAtMove::GetNowParametor() const {
		return m_params[m_currentIndex];
	}

	void Task_CameraAtMove::SetParametors(const vector<Parametor>& parametors) {
		m_params = parametors;
	}

	vector<Task_CameraAtMove::Parametor> Task_CameraAtMove::GetParametors() const {
		return m_params;
	}
}

//endbasecross