/*!
@file ScalePop.cpp
@brief ScalePopのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "ScalePop.h"

#include "Maruyama/Utility/Timer/TimeHelper.h"
#include "Maruyama/Utility/Utility.h"
#include "Mathf.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	パラメータ
	//--------------------------------------------------------------------------------------

	ScalePop_Parametor::ScalePop_Parametor()
		:ScalePop_Parametor(Vec3(1.35f), Vec3(1.0f), 0.0f)
	{}

	ScalePop_Parametor::ScalePop_Parametor(const Vec3& maxScale, const Vec3& returnScale, const float& speed)
		:ScalePop_Parametor(maxScale, returnScale, speed, 0.0f)
	{}

	ScalePop_Parametor::ScalePop_Parametor(const Vec3& maxScale, const Vec3& returnScale, const float& speed, const float& startScale)
		:ScalePop_Parametor(maxScale, returnScale, speed, startScale, maru::DeltaType::Normal)
	{}

	ScalePop_Parametor::ScalePop_Parametor(const Vec3& maxScale, const Vec3& returnScale, const float& speed, const float& startScale, maru::DeltaType deltaType)
		:maxScale(maxScale), returnScale(returnScale), speed(speed), startScale(startScale), deltaType(deltaType)
	{}

	//--------------------------------------------------------------------------------------
	///	スケールポップ本体
	//--------------------------------------------------------------------------------------

	ScalePop::ScalePop(const std::shared_ptr<GameObject>& objPtr) :
		ScalePop(objPtr, Parametor())
	{}

	ScalePop::ScalePop(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor) :
		Component(objPtr),
		m_maxScale(Vec3(parametor.maxScale)),
		m_returnScale(Vec3(parametor.returnScale)),
		m_speed(parametor.speed),
		m_elapsed(parametor.startScale),
		m_deltaType(parametor.deltaType)
	{}

	bool ScalePop::IsStopScale(const Vec3& scale ,const Vec3& targetScale) {
		Vec3 between = targetScale - scale;

		constexpr float betweenSize = 0.1f;

		if (abs(between.x) <= betweenSize &&
			abs(between.y) <= betweenSize)
		{  //最大スケールになったら。
			return true;
		}

		return false;
	}

	void ScalePop::ScaleUpdate() {
		if (m_speed == 0.0f) {
			return;
		}

		auto delta = maru::TimeHelper::GetElapsedTime(m_deltaType);
		m_elapsed += delta * m_speed;

		auto rad = sinf(m_elapsed);

		auto setScale = m_maxScale * rad;

		if (m_elapsed >= XM_PIDIV2) {   //一度最大になったら
			if (IsStopScale(setScale, m_returnScale)) {
				setScale = m_returnScale;
				m_elapsed = 0.0f;
				m_speed = 0.0f;
			}
		}

		transform->SetScale(setScale);
	}

	void ScalePop::OnUpdate() {
		ScaleUpdate();
	}

}

//endbasecross