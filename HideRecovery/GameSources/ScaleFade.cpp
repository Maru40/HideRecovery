/*!
@file ScaleFade.cpp
@brief ScaleFade実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "ScaleFade.h"

namespace basecross {

	ScaleFade::ScaleFade(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor)
		:Component(objPtr), m_param(parametor)
	{}

	void ScaleFade::OnUpdate() {
		if (IsStopScale(transform->GetScale(), m_param.targetScale)) {
			return;
		}

		UpdateScale();

		if (IsStopScale(transform->GetScale(), m_param.targetScale)) {
			if (m_param.endFunction) {
				m_param.endFunction();
			}
		}
	}

	void ScaleFade::UpdateScale() {
		auto delta = App::GetApp()->GetElapsedTime();

		auto scale = transform->GetScale();
		scale += m_param.direct * m_param.speed * delta;

		transform->SetScale(scale);
	}

	bool ScaleFade::IsStopScale(const Vec3& scale, const Vec3& targetScale) const {
		Vec3 between = targetScale - scale;

		float betweenSize = 0.1f;

		if (abs(between.x) <= betweenSize &&
			abs(between.y) <= betweenSize &&
			abs(between.z) <= betweenSize)
		{  //最大スケールになったら。
			return true;
		}

		return false;
	}

}