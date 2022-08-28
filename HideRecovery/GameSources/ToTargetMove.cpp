/*!
@file ToTargetMove.cpp
@brief ToTargetMove実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "ToTargetMove.h"

namespace basecross {

	void ToTargetMove::OnUpdate()
	{
		if (!m_isMove)
		{
			return;
		}

		auto delta = App::GetApp()->GetElapsedTime();
		auto obj = GetGameObject();
		
		auto trans = obj->GetComponent<Transform>();
		auto objPos = trans->GetPosition();
		auto moveVec = Lerp::CalculateLerp(objPos, m_targetPosition, 0.0f, 1.0f, m_speed * delta, Lerp::rate::Linear);
		trans->SetPosition(moveVec);

		auto toTarget = m_targetPosition - moveVec;
		if (toTarget.length() <= 0.1f)  //ある程度ターゲットまで近づいたら。
		{
			m_isMove = false;  //移動をやめる。
			trans->SetPosition(m_targetPosition);

			m_isMoveEnd = true;

			if (m_endFunction) {
				m_endFunction();
			}
		}
	}

	void ToTargetMove::MoveStart(const Vec3& targetPosition, const std::function<void()>& endFunction)
	{
		m_targetPosition = targetPosition;
		m_isMove = true;
		m_isMoveEnd = false;
		m_endFunction = endFunction;
	}

}

//endbasecross