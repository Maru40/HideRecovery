/*!
@file StartCamera.cpp
@brief StartCameraのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "StartCamera.h"
#include "Mathf.h"

#include "GameStageBase.h"

#include "MaruUtility.h"

#include "PlayerInputer.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	カメラの移動管理パラメータ
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
	///	開始カメラ演出用のパラメータ
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
	///	開始演習用のカメラ
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

		//終了のお知らせ
		if (m_params.size() <= m_index) {
			return; //終了
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
			if (IsEnd()) {  //終了だったら
				ChangeCamera();
			}
			else {
				SetEye(m_params[m_index].eye.startPosition);
				SetAt(m_params[m_index].at.startPosition);
			}
		}

	}

	std::shared_ptr<GameStageBase> StartCamera::GetGameStage() {
		//現在nullptrで強制終了中
		return nullptr;

		auto& app = App::GetApp();           // アプリケーションオブジェクトを取得
		auto scene = app->GetScene<Scene>(); // アプリオブジェからシーンを取得
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
		//終了,MainCameraに切り替え
		auto& app = App::GetApp(); // アプリケーションオブジェクトを取得
		auto scene = app->GetScene<Scene>(); // アプリオブジェからシーンを取得
		auto stage = GetGameStage();

		if (stage) {
			stage->ChangeMainCamera();
		}
	}

}

//endbasecross