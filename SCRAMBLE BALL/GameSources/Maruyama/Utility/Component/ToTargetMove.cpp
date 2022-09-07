/*!
@file ToTargetMove.cpp
@brief ToTargetMove����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "ToTargetMove.h"

namespace basecross {

	ToTargetMove::ToTargetMove(const std::shared_ptr<GameObject>& obj) :
		ToTargetMove(
			obj,
			Vec3(0.0f),
			5.0f)
	{}

	ToTargetMove::ToTargetMove(const std::shared_ptr<GameObject>& obj,
		const Vec3& m_targetPosition,
		const float speed
	) :
		Component(obj),
		m_targetPosition(m_targetPosition),
		m_speed(speed)
	{}

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
		if (toTarget.length() <= 0.1f)  //������x�^�[�Q�b�g�܂ŋ߂Â�����B
		{
			m_isMove = false;  //�ړ�����߂�B
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