/*!
@file ToTargetMove.h
@brief ToTargetMove�X�e�[�W
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class ToTargetMove : public Component
	{
		bool m_isMove = false;
		bool m_isMoveEnd = false;

		Vec3 m_targetPosition;  //�ړI�n
		float m_speed;          //�ړ��X�s�[�h

		std::function<void()> m_endFunction = nullptr;

	public:

		ToTargetMove(const std::shared_ptr<GameObject>& obj);

		ToTargetMove(const std::shared_ptr<GameObject>& obj,
			const Vec3& m_targetPosition,
			const float speed
		);

		void OnUpdate() override;

		void MoveStart() { MoveStart(m_targetPosition); }
		void MoveStart(const Vec3& targetPosition, const std::function<void()>& endFunction = nullptr);


		//�A�N�Z�b�T
		void SetTargetPos(const Vec3& position) {
			m_targetPosition = position;
		}

		bool IsMoveEnd(){
			return m_isMoveEnd;
		}

		void SetIsMoveEnd(bool b) {
			m_isMoveEnd = b;
		}

		void SetSpeed(const float speed) {
			m_speed = speed;
		}

		void ForceStop() {
			m_isMove = false;
			m_isMoveEnd = true;
		}
	};



}

//endbasecross